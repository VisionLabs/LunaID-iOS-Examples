//
//  DBrainDocumentType.swift
//  OCR
//
//  Created by michael on 20.11.2023.
//

import Foundation

public struct DBrainDocumentType: DocumentType {
    
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

