//
//  OCRRegulaResult.swift
//  OCR
//
//  Created by cipher on 23.02.2023.
//

import Foundation
import OCRREGULA
import DocumentReader

public struct OCRRegulaResult: OCRResult {
    /// List of recognized document types.
    public let documentType: [DocumentType]
    /// List of recognized text fields.
    public let textFields: [OCRResultTextField]
    /// List of recognized image fields.
    public let imageFields: [OCRResultImageField]
    
    /// :nodoc:
    init(result: DocumentReaderResults) {
        documentType = result.documentType?.map({ RegulaDocumentType(mask: $0.dType.typeName,
                                                               name: $0.name ?? $0.dType.typeName) }) ?? []
        textFields = result.textResult.fields.compactMap({ $0.asField() })
        imageFields = result.graphicResult.fields.map({ $0.asField() })
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
        guard let field = imageFields.first(where: { $0.typeName == "Portrait" }) else { return nil }
        return field
    }
    
}
