//
//  SmartIDImageField+Ext.swift
//  OCR
//
//

import Foundation
import OCRSE

extension SmartIDImageField {
    
    func asField() -> OCRResultImageField? {
        return OCRResultImageField(typeName: name, localizedTypeName: nil, image: value.uiImage, isAccepted: isAccepted())
    }
    
}
