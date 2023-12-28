//
//  DocumentType.swift
//  OCR
//
//

import Foundation

/// Document type.
public protocol DocumentType {
    
    /// Document type mask.
    var mask: String { get }
    /// Document type name.
    var name: String { get }
    
}
