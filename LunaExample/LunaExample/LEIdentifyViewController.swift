//
//  LAFlowOCRViewController.swift
//  LunaMobile
//
//  Created by IVAN CHIRKOV on 16.04.2021.
//
//

import UIKit
import LunaCore
import LunaWeb
import LunaCamera

typealias IdentifyResultHandler = (FaceResultType) -> Void

enum FaceResultType {
    /// Получили данные лица по запросу
    case success(LunaWeb.APIv6.Face?, LunaCore.LCBestShot)
    /// Не запрашивали данные лица, отменяем pаспознавание
    case cancel
}

class LEIdentifyViewController: UIViewController, LMCameraDelegate {
    
    private let SideOffset: CGFloat = 16
    private let biometricInstructionsView = LEBiometricInstructionsView(frame: .zero)
    private let activityIndicator = LEActivityIndicatorView(frame: .zero)

    public var resultBlock: IdentifyResultHandler?
    public var configuration: LCLunaConfiguration
    
    private var livenessAPI = LunaWeb.LivenessAPIv6(configuration: LCLunaConfiguration()) { _ in
        guard let platformToken = LCLunaConfiguration().platformToken else { return [:] }
        return [APIv6Constants.Headers.authorization.rawValue: platformToken]
    }
    
    private var lunaAPI: LunaWeb.APIv6 = {
        LunaWeb.APIv6(lunaAccountID: LCLunaConfiguration().lunaAccountID,
                      lunaServerURL: LCLunaConfiguration().lunaPlatformURL) { _ in
            guard let platformToken = LCLunaConfiguration().platformToken else { return [:] }
            return [APIv6Constants.Headers.authorization.rawValue: platformToken]
        }
    }()

    private let DeepFake_Mouth_Occlusion_HandlerID = "94cca121-0d78-426c-aa69-dfad6f4177ab"

    init(configuration: LCLunaConfiguration) {
        self.configuration = configuration
        super.init(nibName: nil, bundle: nil)
    }

    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    /// Список face id для верификаци
    private var faceIDs: [String] = []

    init(faceIDs: [String], configuration: LCLunaConfiguration) {
        self.faceIDs = faceIDs
        self.configuration = configuration

        super.init(nibName: nil, bundle: nil)
    }

    override func loadView() {
        super.loadView()
        
        createLayout()
    }

    //  MARK: - Routine -
    
    private func closeViewController(animated: Bool) {
        //  hide LMCameraViewControllerProtocol
        dismiss(animated: animated)
        
        //  jumpt to Main screen of application
        navigationController?.popViewController(animated: animated)
    }
    
    private func launchCamera() {
        let cameraViewController = LMCameraBuilder.viewController(delegate: self,
                                                                  configuration: configuration,
                                                                  livenessAPI: livenessAPI,
                                                                  canRecordVideo: configuration.videoRecordLength > 0)

        cameraViewController.dismissHandler = { [weak self] in
            //  MARK: close camera becase dismissHandler was launched
            self?.closeViewController(animated: true)
        }
        cameraViewController.modalPresentationStyle = .fullScreen
        self.present(cameraViewController, animated: true)
    }
    
    private func activateLicense(completion: (Bool, Error?) -> Void) {
        let configuration = LCLunaConfiguration()
        configuration.plistLicenseFileName = LunaCore.kDefaultLicensePlist
        if let error = configuration.activateLicense(withPillar: true) {
            #if LUNA_LOG_SWIFT
            LCSafeLoggerWrapper.logInfo(message: "LMCameraDelegate >>> activating license error \(error.localizedDescription)")
            #endif
            completion(false, error)
            return
        } else {
            completion(true, nil)
        }
    }
    
    private func createLayout() {
        view.accessibilityIdentifier = "identify_start_screen"
        view.backgroundColor = .white
        
        biometricInstructionsView.translatesAutoresizingMaskIntoConstraints = false
        view.addSubview(biometricInstructionsView)
        biometricInstructionsView.cancelButtonHandler = { [weak self] in
            //  MARK: close camera becase cancel button was tapped on Identify screen
            self?.closeViewController(animated: true)
        }
        biometricInstructionsView.captureBiometricButtonHandler = { [weak self] in
            self?.activateLicense() { succes, error in
                if succes {
                    self?.launchCamera()
                } else {
                    self?.presentModalError(error?.what() ?? "License error")
                }
            }
        }
        
        activityIndicator.translatesAutoresizingMaskIntoConstraints = false
        view.addSubview(activityIndicator)
        
        NSLayoutConstraint.activate([
            biometricInstructionsView.leadingAnchor.constraint(equalTo: view.leadingAnchor, constant: SideOffset),
            biometricInstructionsView.trailingAnchor.constraint(equalTo: view.trailingAnchor, constant: -SideOffset),
            biometricInstructionsView.topAnchor.constraint(equalTo: view.topAnchor, constant: SideOffset),
            biometricInstructionsView.bottomAnchor.constraint(equalTo: view.bottomAnchor, constant: -SideOffset),
            
            activityIndicator.leadingAnchor.constraint(equalTo: view.leadingAnchor),
            activityIndicator.trailingAnchor.constraint(equalTo: view.trailingAnchor),
            activityIndicator.topAnchor.constraint(equalTo: view.topAnchor),
            activityIndicator.bottomAnchor.constraint(equalTo: view.bottomAnchor),
        ])
    }
    
    private func identifyBestShotWithPlatform(_ bestShots: [LunaCore.LCBestShot]) {
        let bestShotsData = bestShots.compactMap { $0.bestShotData(configuration: configuration, isWarped: true) }
        let livenessByPhoto = configuration.bestShotConfiguration.livenessType == .byPhoto
        let multipartBestShotsEnabled = configuration.multipartBestShotsEnabled

        guard !bestShotsData.isEmpty, livenessByPhoto else {
            cancelBestshotProcessing()
            return
        }

        var handlerID: String = configuration.identifyHandlerID
        var imageType: ImageType = .faceWarpedImage

        if multipartBestShotsEnabled {
            handlerID = DeepFake_Mouth_Occlusion_HandlerID
            imageType = .rawImage
        }

        let query = LunaWeb.EventQuery(bestShotsData: bestShotsData,
                                       imageType: imageType,
                                       aggregateAttributes: multipartBestShotsEnabled)

        lunaAPI.events.generateEvents(handlerID: handlerID, query: query) { [weak self] result in
            switch result {
            case .success(let response):
                DispatchQueue.main.async {
                    guard let resultBlock = self?.resultBlock else {
                        self?.closeViewController(animated: true)
                        return
                    }

                    self?.dismiss(animated: true) {
                        guard let bestShot = bestShots.last else { return }

                        let face = response.events.first?.matches?.first?.candidates.first?.face
                        resultBlock(.success(face, bestShot))
                    }
                }
            case .failure(let error):
                DispatchQueue.main.async {
                    self?.dismiss(animated: false) {
                        self?.presentModalError((error as NSError).localizedDescription)
                    }
                }
            }
        }
    }

    func verifyBestShotWithPlatform(_ bestShot: LunaCore.LCBestShot) {
        let bestShotData = bestShot.bestShotData(configuration: configuration, isWarped: true)

        guard let bestShotData = bestShotData,
              configuration.bestShotConfiguration.livenessType == .byPhoto
        else {
            cancelBestshotProcessing()
            return
        }

        let query = LunaWeb.PerformVerificationQuery(data: bestShotData, faceIDs: faceIDs)

        lunaAPI.verifiers.performVerification(verifyID: configuration.verifyID, query: query) { [weak self] result in

            switch result {
            case .success(let response):
                let verifications = response.images.first?.detections.faceDetections.first?.verifications
                let filteredVerifications = verifications?.filter { $0.status && $0.similarity > 0.8 }
                let maxSimilarityVerification = filteredVerifications?.max(by: { $0.similarity < $1.similarity })

                DispatchQueue.main.async {
                    if let resultBlock = self?.resultBlock {
                        self?.dismiss(animated: true) {
                            resultBlock(.success(maxSimilarityVerification?.face, bestShot))
                        }
                        return
                    }
                    self?.closeViewController(animated: true)
                }

            case .failure(let error):
                DispatchQueue.main.async {
                    self?.dismiss(animated: false) {
                        self?.presentModalError((error as NSError).localizedDescription)
                    }
                }
            }
        }
    }

    private func cancelBestshotProcessing() {
        DispatchQueue.main.async { [weak self] in
            self?.dismiss(animated: true) {
                self?.resultBlock?(.cancel)
            }
        }
    }

    private func displayError(_ error: LunaCamera.LMCameraError, _ videoFile: String?) {
        //  this action is to close all screens and return to root view controller
        let closeAction = LEAlertAction(title: "OK".localized()) { [weak self] in
            guard let self = self else { return }
            self.closeViewController(animated: true)
        }
        
        if case LMCameraError.accessStatus = error {
            let action = LEAlertAction(title: "common.open_app_settings".localized()) { [weak self] in
                guard let self = self else { return }
                self.closeViewController(animated: true)
                UIApplication.openAppSettings()
            }
            self.presentModalError(error.what(), actions: [action])
        } else {
            switch error {
            case .captureDeviceError(let nsError):
                self.presentModalError(nsError.what(), actions: [closeAction])
            case .canceled:
                self.presentModalError(error.what(),
                                       actions: [closeAction],
                                       needTitle: false,
                                       accessibilityIdentifier: "cancelled_identify_alert_vc")
            case .bestShotError(let nsError):
                var accessibilityIdentifier: String?
                let bestShotError = BestShotError(rawValue: nsError.code)

                switch bestShotError {
                case .LIVENESS_ERROR:
                    accessibilityIdentifier = "offline_liveness_error_identify_alert_vc"
                case .PRIMARY_FACE_CRITICAL_LOST:
                    accessibilityIdentifier = "primary_face_critical_lost_error_identify_alert_vc"
                default:
                    break
                }

                self.presentModalError(error.what(),
                                       actions: [closeAction],
                                       accessibilityIdentifier: accessibilityIdentifier)
            case .error(let innerError):
                guard let networkError = innerError as? LunaWeb.NetworkingError else {
                    self.presentModalError(error.what(), actions: [closeAction])
                    return
                }
                
                switch networkError {
                case .livenessPredictionFakeError, .livenessQualityError:
                    self.presentModalError(error.what(),
                                           actions: [closeAction],
                                           needTitle: false,
                                           accessibilityIdentifier: "online_liveness_error_identify_alert_vc")

                default:
                    self.presentModalError(error.what(), actions: [closeAction])
                }
                
            default:
                self.presentModalError(error.what(), actions: [closeAction])
            }
        }
    }

    //  MARK: - LMCameraDelegate -
    
    func bestShot(_ bestShot: LunaCore.LCBestShot, _ videoFile: String?) {
        if faceIDs.isEmpty {
            identifyBestShotWithPlatform([bestShot])
        } else {
            verifyBestShotWithPlatform(bestShot)
        }
    }

    func multipartBestShots(_ bestShots: [LCBestShot], _ videoFile: String?) {
        if faceIDs.isEmpty {
            identifyBestShotWithPlatform(bestShots)
        } else {
            guard let bestShot = bestShots.last else { return }
            verifyBestShotWithPlatform(bestShot)
        }
    }

    func error(_ error: LunaCamera.LMCameraError, _ videoFile: String?) {
        DispatchQueue.main.async { [weak self] in
            guard let self = self else {
                return
            }
            
            //  close camera screen because it was presented as modal
            self.dismiss(animated: false) {
                self.displayError(error, videoFile)
            }            
        }
    }
        
}
