//
//  OCRAPI.swift
//  OCR
//
//  Created by cipher on 23.02.2023.
//

import UIKit

public protocol OCRAPIProtocol: AnyObject {
    
    func showScanner(_ controller: UIViewController,
                     _ delegate: OCRServiceDelegate?,
                     _ documentType: DocumentType?)
    
}
