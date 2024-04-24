//
//  DocumentReaderGraphicField+Ext.swift
//  OCR
//
//

import Foundation
import DocumentReader

extension DocumentReaderGraphicField {
    
    func asField() -> OCRResultImageField {
        return OCRResultImageField(typeName: fieldType.typeName, localizedTypeName: fieldName, image: value, isAccepted: true)
    }
    
}


extension GraphicFieldType {
    
    var typeName: String {
        return raw[rawValue] ?? "Undefined"
    }
    
    private var raw: [Int: String] {
        return [
            201: "Portrait",
            202: "Fingerprint",
            203: "Eye",
            204: "Signature",
            205: "BarCode",
            206: "ProofOfCitizenship",
            207: "DocumentImage",
            209: "ColorDynamic",
            210: "GhostPortrait",
            211: "Stamp",
            212: "Portrait_Of_Child",
            250: "Other",
            300: "FingerLeftThumb",
            301: "FingerLeftIndex",
            302: "FingerLeftMiddle",
            303: "FingerLeftRing",
            304: "FingerLeftLittle",
            305: "FingerRightThumb",
            306: "FingerRightIndex",
            307: "FingerRightMiddle",
            308: "FingerRightRing",
            309: "FingerRightLittle"
        ]
    }
    
}
