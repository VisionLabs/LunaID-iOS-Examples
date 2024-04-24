//
//  Result.swift
//  OCR
//
//  Created by IVAN CHIRKOV on 22.03.2021.
//

import UIKit

/// Document text field.
public struct OCRResultTextField {
    /// Text field type name.
    public var typeName: String
    /// Localized text field type name.
    public let localizedTypeName: String?
    /// Text field value.
    public let value: String
    
    /// :nodoc:
    public var description: String {
        if let localizedTypeName = localizedTypeName {
            return typeName + "(\(localizedTypeName)) : " + value
        }
        return typeName + " : " + value
    }
}

/// Document image field.
public struct OCRResultImageField {
    /// Image field type name.
    public let typeName: String
    /// Localized image field type name.
    public let localizedTypeName: String?
    /// Image.
    public let image: UIImage
    
    public let isAccepted: Bool
}

public protocol OCRResult {
    
    var documentType: [DocumentType] { get }
    var textFields: [OCRResultTextField] { get }
    var imageFields: [OCRResultImageField] { get }
    var description: String  { get }
    
    func isEmpty() -> Bool
    func faceImageField() -> OCRResultImageField?
    
}
