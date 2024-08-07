//
//  PassportFlow.swift
//  DBrainDocumentFlow
//
//  Created by Александрк Бельковский on 07.10.2020.
//

import UIKit

public class PassportFlow: DocumentFlow {
    
    private let keyTitles: [String: String] = [
        "date_of_birth": "Date of birth: ",
        "date_of_issue": "Date of issue: ",
        "first_name": "First name: ",
        "issuing_authority": "Issuing authority: ",
        "other_names": "Other names: ",
        "place_of_birth": "Place of birth: ",
        "series_and_number": "Series and number: ",
        "sex": "Sex: ",
        "subdivision_code": "Subdivision code: ",
        "surname": "Surname: "
    ]

    override var onReciveResult: ((String) -> String?)? {
        get {
            let handler: ((String) -> String?)? = { key in
                return self.title(by: key)
            }
            
            return handler
        }
        set {
            super.onReciveResult = newValue
        }
    }
    
    public static func configure(authorizationToken: String, classificationUrl: URL = DocumentFlow.classificationUrl, recognitionUrl: URL = DocumentFlow.recognitionUrl, fileKey: String = "image") -> PassportFlow {
        return PassportFlow(type: .passport, authorizationToken: authorizationToken, classificationUrl: classificationUrl, recognitionUrl: recognitionUrl, fileKey: fileKey)
    }
    
    override func defaultTrackingRect() -> CGRect {
        let width = UIScreen.main.bounds.width - 50.0 * 2.0
        let height = width * 1.3
            
        let size = CGSize(width: width, height: height)
        let origin = CGPoint(x: 50.0, y: 88.0)
            
        return CGRect(origin: origin, size: size)
    }
    
    private func title(by key: String) -> String? {
        return super.onReciveResult?(key) ?? keyTitles[key] ?? key
    }
    
}
