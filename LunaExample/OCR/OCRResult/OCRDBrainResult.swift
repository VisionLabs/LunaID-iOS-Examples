//
//  OCRDBrainResult.swift
//  OCR
//
//  Created by michael on 20.11.2023.
//

import UIKit
import OCRDBrain

public struct OCRDBrainResult: OCRResult {
    /// List of recognized document types.
    public let documentType: [DocumentType]
    /// List of recognized text fields.
    public let textFields: [OCRResultTextField]
    /// List of recognized image fields.
    public let imageFields: [OCRResultImageField]
    
    /// :nodoc:
    init(result: [RecognitionItem]) {
        guard let firstRecognitionItem = result.first else {
            documentType = [DBrainDocumentType(mask: "")]
            textFields = [OCRResultTextField]()
            imageFields = [OCRResultImageField]()
            return
        }
        
        documentType = [DBrainDocumentType(mask: firstRecognitionItem.docType)]
        var newImageFields = [OCRResultImageField]()
        textFields = firstRecognitionItem.fields.compactMap({ key, value in
            if key != "base64_image" {
                return value.asField(key)
            }
            
            if let url = URL(string: value.text) {
                do {
                    let imageData = try Data(contentsOf: url)
                    if let image = UIImage(data: imageData) {
                        newImageFields.append(OCRResultImageField(typeName: key,
                                                                  localizedTypeName: nil,
                                                                  image: image,
                                                                  isAccepted: true))
                    }
                }
                catch (_) {
                }
            }
            
            return nil
        })
        imageFields = newImageFields
        print("get result")
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
        guard let field = imageFields.first(where: { $0.typeName == "base64_image" }) else { return nil }
        return field
    }
    
}
