//
//  RecognitionField_Ext.swift
//  OCR
//
//  Created by michael on 20.11.2023.
//

import Foundation
import OCRDBrain

extension RecognitionField {
    
    public func asField(_ typeName: String) -> OCRResultTextField? {
        return OCRResultTextField(typeName: typeName, localizedTypeName: nil, value: text)
    }
    
}
