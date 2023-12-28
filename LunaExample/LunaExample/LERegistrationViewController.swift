//
//  RegistrationViewController.swift
//  LunaExample
//
//  Created by IVAN CHIRKOV on 26.10.2020.
//

import UIKit
import LunaCore
import LunaCamera
import LunaWeb
import CoreLocation

class LERegistrationViewController: UIViewController, UITextFieldDelegate {

    private let UserNameTextFieldHeight: CGFloat = 46
    private let CommonSideOffset: CGFloat = 16

    private var activityView = LEActivityIndicatorView(frame: .zero)
    private let usernameField = LETextField(frame: .zero)
    private let usernameReasonLabel = UILabel(frame: .zero)
    
    private lazy var signUpButton: UIBarButtonItem = {
        UIBarButtonItem(title: "registration.sign_up".localized(),
                        style: .done,
                        target: self,
                        action: #selector(signUP))
    }()
    
    private var lunaAPI = LunaWeb.APIv6(lunaAccountID: LCLunaConfiguration().lunaAccountID,
                                        lunaServerURL: LCLunaConfiguration().lunaServerURL,
                                        additionalHeaders: nil)

    public var configuration = LCLunaConfiguration() {
        didSet {
            lunaAPI = APIv6(lunaAccountID: configuration.lunaAccountID,
                            lunaServerURL: configuration.lunaServerURL,
                            additionalHeaders: nil)
        }
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        
        navigationController?.setNavigationBarHidden(false, animated: true)
    }
    
    override func loadView() {
        super.loadView()
        
        createLayout()
    }
    
    //  MARK: - Handlers -
    
    @objc
    private func tapOnView() {
        view.endEditing(true)
    }
    
    @objc
    private func signUP() {
        activityView.startAnimating()
        
        usernameReasonLabel.isHidden = true
        usernameField.isValid = true
        
        let query = GetFacesQuery(userData: usernameField.text, externalIDs: nil, targets: [.faceID, .userData])
        lunaAPI.faces.getFaces(query: query) { result in
            DispatchQueue.main.async { [weak self] in
                self?.activityView.stopAnimating()
                guard let userName = self?.usernameField.text else {
                    return
                }
                
                switch result {
                case .success(let facesResponse):
                    let takenUserNames = facesResponse.faces.compactMap({ $0.userData })
                    // Проверяем занято ли имя пользователя на сервере Luna
                    if takenUserNames.contains(userName) {
                        self?.usernameReasonLabel.isHidden = false
                        self?.usernameField.isValid = false
                        self?.usernameReasonLabel.text = LEServerError.userNameAlreadyTaken.what()
                    } else {
                        self?.launchRegistration(userName)
                    }
                case .failure(let error):
                    self?.presentModalError(error.what())
                }
            }
        }
    }
    
    @objc
    private func validateUserName(_ textField: LETextField) {
        guard let text = textField.text else {
            signUpButton.isEnabled = false
            return
        }
        
        if text.isEmpty {
            signUpButton.isEnabled = false
            return
        }
        
        do {
            let isValid = try textField.text.isValidAsName()
            textField.isValid = isValid
            usernameReasonLabel.isHidden = isValid
            signUpButton.isEnabled = isValid
        } catch {
            textField.isValid = false
            signUpButton.isEnabled = false
            usernameReasonLabel.isHidden = false
            usernameReasonLabel.text = error.what()
        }
    }
    
    
    //  MARK: - Routine -
    
    private func createLayout() {
        view.backgroundColor = .white
        let tapGesture = UITapGestureRecognizer(target: self, action: #selector(tapOnView))
        view.addGestureRecognizer(tapGesture)
        
        usernameField.translatesAutoresizingMaskIntoConstraints = false
        usernameField.delegate = self
        usernameField.placeholder = "success.user_name".localized()
        usernameField.addTarget(self, action: #selector(validateUserName), for: .editingChanged)
        usernameField.autocorrectionType = .no
        view.addSubview(usernameField)
        
        usernameReasonLabel.translatesAutoresizingMaskIntoConstraints = false
        usernameReasonLabel.isHidden = true
        view.addSubview(usernameReasonLabel)
        
        signUpButton.isEnabled = false
        navigationItem.setRightBarButton(signUpButton, animated: false)

        NSLayoutConstraint.activate([
            usernameField.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor, constant: CommonSideOffset),
            usernameField.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: CommonSideOffset),
            usernameField.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -CommonSideOffset),
            usernameField.heightAnchor.constraint(equalToConstant: UserNameTextFieldHeight),
            
            usernameReasonLabel.topAnchor.constraint(equalTo: usernameField.bottomAnchor, constant: CommonSideOffset),
            usernameReasonLabel.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: CommonSideOffset),
            usernameReasonLabel.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -CommonSideOffset)
        ])
    }
    
    private func launchOCR(_ bestShot: LunaCore.LCBestShot, _ userName: String?) {
        let viewController = LEOCRViewController()
        viewController.resultBlock = { [weak self] ocrResult in
            guard let ocrResult = ocrResult else {
                let failedViewController = LEResultViewController()
                failedViewController.resultTitle = "ocr.failed".localized()
                failedViewController.resultImageName = "fail"
                failedViewController.closeHandler = { [weak self] in
                    self?.navigationController?.popToRootViewController(animated: true)
                }
                self?.navigationController?.pushViewController(failedViewController, animated: true)
                return
            }
            
            let ocrResultsViewController = LEOCRResultsViewController()
            ocrResultsViewController.configureResults(bestShot, ocrResult, "buttons.do_actions.registration".localized())
            ocrResultsViewController.continueButtonHandler = { [weak self] error in
                let resultViewController = LEResultViewController()
                resultViewController.resultTitle = "registration.failed".localized()
                resultViewController.resultImageName = "fail"
                if let error = error {
                    resultViewController.resultTitle = (userName ?? "") + "\n\n"
                    resultViewController.resultTitle += "registration.failed".localized()
                    resultViewController.resultImageName = "fail"
                }
                else {
                    resultViewController.resultTitle = (userName ?? "") + "\n\n"
                    resultViewController.resultTitle += "registration.successful".localized()
                    resultViewController.resultImageName = "success"
                }
                resultViewController.closeHandler = { [weak self] in
                    self?.navigationController?.popToRootViewController(animated: true)
                }
                self?.navigationController?.pushViewController(resultViewController, animated: true)
            }
            self?.navigationController?.pushViewController(ocrResultsViewController, animated: true)
        }
        navigationController?.pushViewController(viewController, animated: true)
    }
    
    private func executeRegistration(_ userName: String, _ bestShot: LunaCore.LCBestShot) {
        guard let bestShotData = bestShot.bestShotData(configuration: configuration, isWarped: true) else {
            showFailedScreenWithMessage(nil)
            return
        }

        let query = EventQuery(data: bestShotData,
                               imageType: .faceWarpedImage,
                               externalID: nil,
                               userData: userName
        )

        lunaAPI.events.generateEvents(handlerID: configuration.registrationHandlerID, query: query) { result in
            DispatchQueue.main.async { [weak self] in
                guard let self = self else { return }
                switch result {
                case .success(let response):
                    if (self.configuration.ocrEnabled) {
                        self.launchOCR(bestShot, userName)
                        return
                    }
                    else {
                        let resultViewController = LEResultViewController()
                        resultViewController.resultTitle = userName + "\n\n"
                        resultViewController.resultTitle += "registration.successful".localized()
                        resultViewController.resultImageName = "success"
                        resultViewController.closeHandler = { [weak self] in
                            self?.navigationController?.popToRootViewController(animated: true)
                        }
                        self.navigationController?.pushViewController(resultViewController, animated: true)
                    }
                case .failure(let error):
                    self.showFailedScreenWithMessage((error as NSError).localizedDescription)
                }
            }
        }
    }
    
    private func showFailedScreenWithMessage(_ message: String?) {
        let resultViewController = LEResultViewController()
        
        resultViewController.resultTitle = message ?? "registration.failed".localized()
        resultViewController.resultImageName = "fail"
        resultViewController.closeHandler = { [weak self] in
            self?.navigationController?.popToRootViewController(animated: true)
        }
        
        navigationController?.pushViewController(resultViewController, animated: true)
    }
    
    /// Start camera to recognize face. On this stage userName is the one that already checked and does not registered as user name for another user of Luna Platform
    /// - Parameter userName: checked and unused  user name
    private func launchRegistration(_ userName: String) {
        let identifyViewController = LEIdentifyViewController()
        identifyViewController.configuration = configuration
        identifyViewController.resultBlock = { [weak self] face, bestShot in
            if let face = face {
                self?.showFailedScreenWithMessage("registration.face.failed".localized())
            }
            else {
                self?.executeRegistration(userName, bestShot)
            }
        }
        navigationController?.pushViewController(identifyViewController, animated: true)
    }
    
    //  MARK: - UITextFieldDelegate -
    
    public func textFieldShouldReturn(_ textField: UITextField) -> Bool {
        do {
            let isValid = try textField.text.isValidAsName()
            signUP()
            return isValid
        } catch {
            return false
        }
    }
    
}
