//
//  OCRSEResult.swift
//  OCR
//
//  Created by cipher on 23.02.2023.
//

import Foundation
import OCRSE

public struct OCRSEResult: OCRResult {
    /// List of recognized document types.
    public let documentType: [DocumentType]
    /// List of recognized text fields.
    public let textFields: [OCRResultTextField]
    /// List of recognized image fields.
    public let imageFields: [OCRResultImageField]
    
    /// :nodoc:
    init(result: SmartIDRecognitionResult) {
        documentType = [SEDocumentType(mask: result.getDocumentType())]
        textFields = result.getStringFields().compactMap({ $1.asField() })
        imageFields = result.getImageFields().compactMap({ $1.asField() })
    }
    
    /// :nodoc:
    public var description: String {
        let header = "Document Type: \(documentType.map({ $0.mask + "(\($0.name))" }).joined(separator: ", "))\n\n"
        return header + textFields.reduce("", { $0 + "\n\n" + $1.description })
    }
    
    /// Returns true if no text fields were detected.
    public func isEmpty() -> Bool {
        textFields.isEmpty
    }
    
    /// Returns an image field with a face.
    public func faceImageField() -> OCRResultImageField? {
        fatalError("You should not create object of pure OCRResult struct! Please use classes OCRSEResult or OCRRegulaResult")
        guard let field = imageFields.first(where: { $0.typeName == "photo" }) else { return nil }
        return field
    }
    
}
