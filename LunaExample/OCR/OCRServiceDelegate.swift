//
//  OCRDelegate.swift
//  OCR
//
//  Created by IVAN CHIRKOV on 24.03.2021.
//

import Foundation

/// OCR delegate.
///
/// Implement this protocol to get the OCR output.
public protocol OCRServiceDelegate: AnyObject {
    
    /// Called when OCR initialization starts.
    func ocrStartInitialization()
    
    
    /// Called when OCR initialization is complete.
    ///
    /// If initialization fails, you will get an error.
    /// - Parameter error: Initialization error.
    func ocrFinishInitialization(error: Swift.Error?)
    
    
    /// Called when receiving OCR result.
    /// - Parameter result: OCR result.
    func ocrResult(result: OCRResult)
    
    
    /// Called when an OCR error occurs.
    /// - Parameter error: OCR Error.
    func ocrError(error: Swift.Error)
    
}

public extension OCRServiceDelegate {
    
    func ocrStartInitialization() {
        
    }
    
    func ocrFinishInitialization(error: Swift.Error?) {
        
    }

}
