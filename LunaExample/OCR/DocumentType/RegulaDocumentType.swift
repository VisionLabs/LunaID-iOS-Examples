//
//  RegulaDocumentType.swift
//  OCR
//
//  Created by cipher on 23.02.2023.
//

import Foundation

public struct RegulaDocumentType: DocumentType {
    /// Document type mask.
    public let mask: String
    /// Document type name.
    public let name: String
    
    /// :nodoc:
    public init(mask: String) {
        self.mask = mask
        self.name = mask
    }
    
    /// :nodoc:
    init(mask: String, name: String) {
        self.mask = mask
        self.name = name
    }
    
}
