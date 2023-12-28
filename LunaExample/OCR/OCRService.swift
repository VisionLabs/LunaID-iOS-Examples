//
//  OCR.swift
//  OCR
//
//  Created by IVAN CHIRKOV on 22.03.2021.
//

import UIKit

/// The main object for working with OCR.
public class OCRService: NSObject {
    
    //  injections
    public var ocrAPI: OCRAPIProtocol
    
    public init(ocrAPI: OCRAPIProtocol) {
        self.ocrAPI = ocrAPI
    }
    
    public func showScanner(_ controller: UIViewController,
                            _ delegate: OCRServiceDelegate?,
                            _ documentType: DocumentType? = nil) {
        ocrAPI.showScanner(controller, delegate, documentType)
    }
    
}
