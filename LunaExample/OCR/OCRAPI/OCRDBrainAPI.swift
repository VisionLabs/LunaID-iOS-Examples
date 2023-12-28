//
//  OCRDBrainAPI.swift
//  OCR
//
//  Created by michael on 20.11.2023.
//

import UIKit
import OCRDBrain

public class OCRBdrainAPI: OCRAPIProtocol {

    private let DBrainLicenseToken = "eJIwkXCqh44LWXN0jgjsHjGK"
    
    private weak var delegate: OCRServiceDelegate?
    
    public init(delegate: OCRServiceDelegate? = nil) {
        self.delegate = delegate
    }
    
    /// Present the OCR scanner to the specified view controller.
    /// - Parameter controller: The view controller on which the scanner will be presented.
    public func showScanner(_ controller: UIViewController,
                            _ delegate: OCRServiceDelegate?,
                            _ documentType: DocumentType? = nil) {
        self.delegate = delegate
        delegate?.ocrStartInitialization()

        let onEndFlow: ([RecognitionItem]) -> Void = { [weak self] dbrainResult in
            delegate?.ocrResult(result: OCRDBrainResult(result: dbrainResult))
            controller.dismiss(animated: true)
        }
        
        let keyTitles: [String: String] = [
            "some_key": "Some key"
        ]
        
        let onReceiveResult: ((_ key: String) -> String?) = { key in
            return keyTitles[key] ?? key
        }
        
        let onReceiveDocumentType: ((_ type: String) -> (title: String, isEnabled: Bool)) = { type in
            return (type.replacingOccurrences(of: "_", with: " ").capitalized, type != "not_document")
        }
        
        let side = UIScreen.main.bounds.width - 50.0 * 2.0
        let size = CGSize(width: side, height: side)
        let origin = CGPoint(x: 50.0, y: 88.0)
        
        let documentFlow = DocumentFlow.configure(type: .selectable, authorizationToken: DBrainLicenseToken)
            .with(onEndFlow: onEndFlow)
            .with(onReciveResult: onReceiveResult)
            .with(onReciveDocumentType: onReceiveDocumentType)
            .with(lumaDiffCoefficient: 0.70)
            .with(expectedSizeKb: 1000)
            .with(trackingRect: CGRect(origin: origin, size: size))
            .withDebugViews()
            .withResult()
            .build()
        
        let viewController = documentFlow.start()
        viewController.modalPresentationStyle = .fullScreen

        delegate?.ocrFinishInitialization(error: nil)
        controller.present(viewController, animated: true)

        
//        guard let licenseData = loadLicenseData() else {
//            delegate?.ocrError(error: OCRREGULA.Error.noLicense)
//            return
//        }
//        
//        Regula.shared.initialize(licenseData: licenseData) { [weak self] (licenseLoadResult) in
//            switch licenseLoadResult {
//            case .success:
//                self?.delegate?.ocrFinishInitialization(error: nil)
//                Regula.shared.showScanner(on: controller) { result in
//                    guard let result = result else {
//                        self?.delegate?.ocrError(error: OCRError.notRecognized)
//                        return
//                    }
//                    
//                    switch result {
//                    case .success(let regulaResult):
//                        let delegateResult = OCRRegulaResult(result: regulaResult)
//                        self?.delegate?.ocrResult(result: delegateResult)
//                    case .failure(let error):
//                        self?.delegate?.ocrError(error: error)
//                    }
//                }
//            case .failure(let error):
//                self?.delegate?.ocrFinishInitialization(error: error)
//            }
//        }
    }
        
}
