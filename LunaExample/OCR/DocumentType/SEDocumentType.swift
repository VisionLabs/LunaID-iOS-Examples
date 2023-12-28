//
//  SEDocumentType.swift
//  OCR
//
//  Created by cipher on 23.02.2023.
//

import Foundation

public struct SEDocumentType: DocumentType {
    /// Document type mask.
    public let mask: String
    /// Document type name.
    public let name: String
    
    /// :nodoc:
    public init(mask: String) {
        self.mask = mask
        let docTypes = SEDocumentType.docTypesDescription()
        self.name = docTypes[mask] ?? mask
    }
    
    /// :nodoc:
    init(mask: String, name: String) {
        self.mask = mask
        self.name = name
    }
    
    /// :nodoc:
    static func documentTypes(supportedDocTypes: [String]) -> [DocumentType] {
        let docTypes = docTypesDescription()
        return supportedDocTypes.map({ key in
            if let value = docTypes[key] {
                return SEDocumentType(mask: key, name: value)
            } else {
                return SEDocumentType(mask: key, name: key)
            }
        })
    }
    
    /// :nodoc:
    private static func docTypesDescription() -> [String: String] {
        let bundle = Bundle(for: OCRService.self)
        if let filepath = bundle.path(forResource: "doctypesDescriptions", ofType: "csv"),
           let string = try? String(contentsOfFile: filepath) {
            return string.split(whereSeparator: \.isNewline).dropFirst().compactMap { (line) -> [String: String]? in
                let split = line.split(separator: ";")
                if let id = split.first, let name = split.last {
                    return [String(id): String(name)]
                }
                return nil
            }.reduce([:]) { (dict, pair) in
                return dict.merging(pair) { first, _ in first }
            }
        }
        
        return [:]
    }
    
}
