//
//  DocumentViewController.swift
//  dbraion
//
//  Created by Александрк Бельковский on 03/06/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import UIKit
import AVFoundation

class DocumentViewController: UIViewController {
    
    var type: DocumentFlow.DocumentType = .empty
    
    var trackingRect: CGRect!
    var shouldShowDebugElements: Bool = false
    var lumaDiffCoefficient: CGFloat = 1.0
    
    var authorizationToken: String!
    var classificationUrl: URL!
    var recognitionUrl: URL!
    var fileKey: String!
    var expectedSizeKb: Int!
    var displayResult: Bool!
    
    var onEndFlow: (([RecognitionItem]) -> Void)?
    var onReciveResult: ((_ key: String) -> String?)?
    var onReciveDocumentType: ((_ type: String) -> (title: String, isEnabled: Bool))?
    
    private var shouldApplyByHistogram: Bool = true
    
    private lazy var cameraService: CameraService = {
        let cameraService = CameraService(with: mainView.cameraPreviewView, cameraPosition: .back)
        cameraService.delegate = self
        
        return cameraService
    }()
    
    private var mainView: BaseDocumentView {
        return view as! BaseDocumentView
    }
    
    // MARK: - Lifecycle
    
    override var prefersStatusBarHidden: Bool {
        return true
    }
    
    deinit {
        NotificationCenter.default.removeObserver(self)
    }
    
    override func loadView() {
        view = GeneralDocumentView(trackingRect: trackingRect)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        mainView.photoButton.addTarget(self, action: #selector(photoButtonClicked), for: .touchUpInside)
        
        NotificationCenter.default.addObserver(self, selector: #selector(didEnterBackground), name: UIApplication.didEnterBackgroundNotification, object: nil)
        NotificationCenter.default.addObserver(self, selector: #selector(willEnterForeground), name: UIApplication.willEnterForegroundNotification, object: nil)
    }
    
    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        
        cameraService.requestAccess { [weak self] granded in
            guard granded else {
                return
            }
            self?.start()
        }
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(animated)
        
        cameraService.stop()
    }
    
    // MARK: - Camera
    
    private func start() {
        let cameraTextHandler = CameraTextHandler()
        cameraTextHandler.textDelegate = self
        cameraService.cameraHandler = cameraTextHandler
        
        cameraService.start()
    }
    
    // MARK: - Handler
    
    @objc private func photoButtonClicked(_ sender: LoadingButton) {
        sender.isLoading = true
        cameraService.takePhoto()
    }
    
    @objc private func didEnterBackground(_ sender: Any) {
        cameraService.stop()
    }
    
    @objc private func willEnterForeground(_ sender: Any) {
        cameraService.start()
    }
    
    // MARK: - Content
    
    private func routeResult(image: UIImage) {
        let loader = DocumentLoader(classificationUrl: classificationUrl, recognitionUrl: recognitionUrl, authorizationToken: authorizationToken, fileKey: fileKey, expectedSizeKb: expectedSizeKb)
        
        cameraService.stop()
        
        var recognitionType: String?
        
        switch type {
        case .passport:
            recognitionType = "passport_main"
        case .custom(let t):
            recognitionType = t
        case .empty:
            recognitionType = nil
        case .driverLicence:
            recognitionType = nil
        case .selectable:
            recognitionType = nil
        }
        
        loader.recognition(image: image, type: recognitionType) { [weak self] result in
            switch result {
            case .success(let response):
                if self?.displayResult == true {
                    self?.routeToPasportSuccess(response: response)
                } else {
                    self?.onEndFlow?(response.items)
                }
            case .failure(let error):
                let title: String
                
                if let error = error as? DocumentLoaderError {
                    switch error {
                    case .invalidParse:
                        title = "Data processing error"
                    case .serverError:
                        title = "Server error"
                    }
                } else {
                    title = error.localizedDescription
                }
                
                self?.routeToFailure(title: title)
            }
        }
        
    }

    private func routeToSelectableResult(image: UIImage) {
        let loader = DocumentLoader(classificationUrl: classificationUrl, recognitionUrl: recognitionUrl, authorizationToken: authorizationToken, fileKey: fileKey, expectedSizeKb: expectedSizeKb)
        
        cameraService.stop()
        
        loader.classification(image: image) { [weak self] result in
            switch result {
            case .success(let response):
                self?.routeToSelectableSuccess(response: response, loader: loader)
            case .failure(let error):
                let title: String
                
                if let error = error as? DocumentLoaderError {
                    switch error {
                    case .invalidParse:
                        title = "Data processing error"
                    case .serverError:
                        title = "Server error"
                    }
                } else {
                    title = error.localizedDescription
                }
                
                self?.routeToFailure(title: title)
            }
        }

    }
    
    private func routeToPasportSuccess(response: RecognitionResponse) {
        let rocumentResultViewController = DocumentResultViewController()
        rocumentResultViewController.respone = response
        rocumentResultViewController.onReciveResult = onReciveResult
        rocumentResultViewController.onEndFlow = onEndFlow
        
        present(childViewController: rocumentResultViewController) { [weak self] _ in
            self?.mainView.photoButton.isLoading = false
            self?.cameraService.stop()
        }
    }
    
    private func routeToDocumentResult(response: RecognitionResponse) {
        let rocumentResultViewController = DocumentResultViewController()
        rocumentResultViewController.respone = response
        rocumentResultViewController.onReciveResult = onReciveResult
        rocumentResultViewController.onEndFlow = onEndFlow
        
        present(childViewController: rocumentResultViewController) { _ in }
    }

    //=====================================================================

    private func routeToResult(response: RecognitionResponse) {
        let rocumentResultViewController = DocumentResultViewController()
        rocumentResultViewController.respone = response
        rocumentResultViewController.onReciveResult = onReciveResult
        rocumentResultViewController.onEndFlow = onEndFlow
        
        present(childViewController: rocumentResultViewController) { _ in }
    }
    
    private func convertBase64StringToImage (imageBase64String: String) -> UIImage? {
        if let url = URL(string: imageBase64String) {
            do {
                let imageData = try Data(contentsOf: url)
                let image = UIImage(data: imageData)
                return image
            } catch {
                print(error)
            }
        }
        
        return nil
    }

    private func startImageRecognition(_ value: ClassificationItem?, _ loader: DocumentLoader) {
        guard let value = value else {
            return
        }
        
        guard let image = convertBase64StringToImage(imageBase64String: value.crop) else {
            return
        }
        
        loader.recognition(image: image, type: value.document.type) { [weak self] result in
            switch result {
            case .success(let response):
                var updatedResponse: RecognitionResponse = RecognitionResponse(items: response.items,
                                                                               code:response.code,
                                                                               message: response.message,
                                                                               errno: response.errno)
                if let firstItem: RecognitionItem = updatedResponse.items.first {
                    var mutableFields = firstItem.fields
                    mutableFields["base64_image"] = RecognitionField(text: value.crop, confidence: 1)
                    updatedResponse.items[0] = RecognitionItem(docType: firstItem.docType,
                                                               fields: mutableFields)
                }
                
                if self?.displayResult == true {
                    self?.routeToResult(response: updatedResponse)
                } else {
                    self?.onEndFlow?(response.items)
                }
            case .failure(let error):
                print(error)
            }
        }
  
    }
    
//=====================================================================
    
    private func routeToSelectableSuccess(response: ClassificationResponse, loader: DocumentLoader) {
//        let viewController: DocumentSelectViewController = DocumentSelectViewController()
//        viewController.onEndFlow = onEndFlow
//        viewController.onReciveResult = onReciveResult
//        viewController.displayResult = displayResult
//        viewController.loader = loader
                
        var res = response
        
        for (index, i) in res.items.enumerated() {
            var document = i.document
            
            let result = onReciveDocumentType?(document.type)
            
            document.isEnabled = result?.isEnabled ?? true
            document.title = result?.title ?? i.document.type

            res.items[index].document = document
        }
        startImageRecognition(res.items.first, loader)
        
//        viewController.values = res.items
//        
//        viewController.backHandler = { [weak self] in
//            self?.start()
//            guard let vc = self?.children.first(where: { $0 is DocumentSelectViewController }) else {
//                return
//            }
//            self?.remove(childViewController: vc)
//        }
//        
//        present(childViewController: viewController) { [weak self] _ in
//            self?.mainView.photoButton.isLoading = false
//            self?.cameraService.stop()
//        }
    }
    
    private func routeToFailure(title: String) {
        let viewController = UploadFailureViewController()
        viewController.errorText = title
        
        viewController.retakeHandler = { [weak self] in
            self?.start()
            guard let vc = self?.children.first(where: { $0 is UploadFailureViewController }) else {
                return
            }
            self?.remove(childViewController: vc)
            
        }
        
        present(childViewController: viewController) { [weak self] _ in
            self?.mainView.photoButton.isLoading = false
            
            guard let vc = self?.children.first(where: { $0 is DocumentResultViewController }) else {
                return
            }
            vc.willMove(toParent: nil)
            vc.view.removeFromSuperview()
            vc.removeFromParent()
        }
    }
    
}

// MARK: - CameraTextHandlerDelegate
extension DocumentViewController: CameraTextHandlerDelegate {
    
    func cameraTextHandler(_ cameraTextHandler: CameraTextHandler, reciveLuma histogram: [UInt], maximum: UInt, minimum: UInt) {
        let coefficient: CGFloat = min(1.0, lumaDiffCoefficient)
        let borderValue = CGFloat(maximum) * coefficient
        shouldApplyByHistogram = CGFloat(histogram.last ?? 0) < borderValue
        
        if shouldApplyByHistogram {
            mainView.info(isHidden: true)
            mainView.view(apply: true)
        } else {
            let color = UIColor(red: 235.0 / 255.0, green: 0.0 / 255.0, blue: 141.0 / 255.0 , alpha: 1.0)
            let font = UIFont.systemFont(ofSize: 14.0, weight: .regular)
            mainView.setupInfo(text: "To match light", color: color, font: font)
            mainView.info(isHidden: false)
            mainView.view(apply: false)
        }
        
        // Debug elements
        guard shouldShowDebugElements else {
            return
        }
        
        if mainView.histogramView.superview == nil {
            mainView.cameraPreviewView.addSubview(mainView.histogramView)
            mainView.histogramView.heightAnchor.constraint(equalTo: mainView.widthAnchor, multiplier: 0.6).isActive = true
            mainView.histogramView.leadingAnchor.constraint(equalTo: mainView.cameraPreviewView.leadingAnchor).isActive = true
            mainView.histogramView.trailingAnchor.constraint(equalTo: mainView.cameraPreviewView.trailingAnchor).isActive = true
            mainView.histogramView.bottomAnchor.constraint(equalTo: mainView.cameraPreviewView.bottomAnchor).isActive = true
        }
        
        mainView.histogramView.updateLumaLayer(values: histogram, max: maximum)
        mainView.histogramView.updateMaxLayer(borderValue: borderValue, maximum: maximum)
    }
    
}


// MARK: - CameraServiceDelegate
extension DocumentViewController: CameraServiceDelegate {
    
    func cameraService(_ cameraService: CameraService, didTake asset: AVAsset, url: URL) {
        
    }
    
    func cameraService(targetRect cameraService: CameraService) -> CGRect {
        return trackingRect
    }
    
    func cameraService(_ cameraService: CameraService, didTake image: UIImage) {
        switch type {
        case .selectable:
            routeToSelectableResult(image: image)
        default:
            routeResult(image: image)
        }
        
        
//        guard let type = type else {
//            return
//        }
//
//        switch type {
//        case .driverLicence:
//            routeToDriverLicense(image: image)
//        case .selectable:
//            routeToSelectableResult(image: image)
//        case .pasport:
//            routeToPasportResult(image: image)
//        }
    
    }
    
    func cameraService(didStart cameraService: CameraService) {
        mainView.photoButton.isEnabled = true
    }
    
    func cameraService(didStop cameraService: CameraService) {
        mainView.photoButton.isEnabled = false
    }
    
}
