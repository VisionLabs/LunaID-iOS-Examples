//
//  OCRRegulaAPI.swift
//  OCR
//
//  Created by cipher on 25.10.2022.
//

import UIKit
import OCRREGULA

public class OCRRegulaAPI: OCRAPIProtocol {
    
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
        
        guard let licenseData = loadLicenseData() else {
            delegate?.ocrError(error: OCRREGULA.Error.noLicense)
            return
        }

        Regula.shared.initialize(licenseData: licenseData) { [weak self] (licenseLoadResult) in
            switch licenseLoadResult {
            case .success:
                self?.delegate?.ocrFinishInitialization(error: nil)
                Regula.shared.showScanner(on: controller) { result in
                    guard let result = result else {
                        self?.delegate?.ocrError(error: OCRError.notRecognized)
                        return
                    }
                    
                    switch result {
                    case .success(let regulaResult):
                        let delegateResult = OCRRegulaResult(result: regulaResult)
                        self?.delegate?.ocrResult(result: delegateResult)
                    case .failure(let error):
                        self?.delegate?.ocrError(error: error)
                    }
                }
            case .failure(let error):
                self?.delegate?.ocrFinishInitialization(error: error)
            }
        }
    }
    
    private func loadLicenseData() -> Data? {
        guard let dataPath = Bundle.main.path(forResource: "regula.license", ofType: nil) else {
            return nil
        }
        
        return try! Data(contentsOf: URL(fileURLWithPath: dataPath))
    }
        
}
