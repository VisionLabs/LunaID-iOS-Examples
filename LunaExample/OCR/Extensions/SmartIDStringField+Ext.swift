//
//  SmartIDStringField+Ext.swift
//  OCR
//
//

import Foundation
import OCRSE

extension SmartIDStringField {
    
    func asField() -> OCRResultTextField? {
        guard isAccepted() else { return nil }
        return OCRResultTextField(typeName: getName(), localizedTypeName: nil, value: getValue())
    }
    
}
