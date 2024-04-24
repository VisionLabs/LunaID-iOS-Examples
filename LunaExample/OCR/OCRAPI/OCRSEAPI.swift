//
//  OCRSEAPI.swift
//  OCR
//
//  Created by cipher on 25.10.2022.
//

import UIKit
import OCRSE

public class OCRSEAPI: NSObject, OCRAPIProtocol {
    
    /// OCR delegate.
    private weak var delegate: OCRServiceDelegate?
    
    private lazy var smartEngineVC: SmartIDViewControllerSwift = {
        let controller = SmartIDViewControllerSwift()
        
        controller.modalPresentationStyle = .fullScreen
        controller.captureButtonDelegate = controller
        controller.sessionTimeout = 15
        controller.displayZonesQuadrangles = true
        controller.displayDocumentQuadrangle = true
        controller.displayProcessingFeedback = true
        controller.smartIDDelegate = self
        controller.setMode("anycis")
        controller.addEnabledDocTypesMask("*")
        controller.sessionOption(withOptionName: "common.extractTemplateImages", andWithOptionValue: "true")
        
        return controller
    }()
    
    public init(delegate: OCRServiceDelegate? = nil) {
        self.delegate = delegate
    }

    /// Present the OCR scanner to the specified view controller.
    /// - Parameter controller: The view controller on which the scanner will be presented.
    /// - Parameter documentType: Document type. If not specified, the document type will be recognized automatically.
    public func showScanner(_ controller: UIViewController,
                            _ delegate: OCRServiceDelegate?,
                            _ documentType: DocumentType? = nil) {
        self.delegate = delegate
        delegate?.ocrStartInitialization()
        
        if let documentType = documentType {
            smartEngineVC.removeEnabledDocTypesMask("*")
            smartEngineVC.addEnabledDocTypesMask(documentType.mask)
            controller.present(smartEngineVC, animated: true, completion: { [weak self] in
                self?.delegate?.ocrFinishInitialization(error: nil)
            })
        } else {
            controller.present(smartEngineVC, animated: true) { [weak self] in
                self?.delegate?.ocrFinishInitialization(error: nil)
            }
        }
    }
    
}

extension OCRSEAPI: SmartIDViewControllerDelegate {
    
    /// :nodoc:
    public func smartIDViewControllerDidRecognize(_ result: SmartIDRecognitionResult, from buffer: CMSampleBuffer) {
        
    }
    
    /// :nodoc:
    public func smartIDViewControllerDidRecognize(_ result: SmartIDRecognitionResult) {
        guard result.isTerminal() else {
            return
        }
        
        let ocrResult = OCRSEResult(result: result)
        if ocrResult.isEmpty() {
            delegate?.ocrError(error: OCRError.notRecognized)
        } else {
            delegate?.ocrResult(result: ocrResult)
        }
        smartEngineVC.dismiss(animated: true, completion: nil)
    }
    
    /// :nodoc:
    public func smartIDviewControllerDidCancel() {
        smartEngineVC.dismiss(animated: true, completion: nil)
        delegate?.ocrError(error: OCRError.canceled)
    }
    
}
