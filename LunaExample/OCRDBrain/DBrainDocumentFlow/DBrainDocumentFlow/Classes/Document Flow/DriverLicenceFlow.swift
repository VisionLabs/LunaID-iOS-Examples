//
//  DriverLicenceFlow.swift
//  DBrainDocumentFlow
//
//  Created by Александрк Бельковский on 07.10.2020.
//

import UIKit

public class DriverLicenceFlow: DocumentFlow {
    
    private let keyTitles: [String: String] = [
        "name": "Name: ",
        "place_of_birth": "Place of birth: ",
        "number": "Number: ",
        "date_from": "Date from: ",
        "date_end": "Date end: ",
        "place_of_issue": "Place of ssue: ",
        "Patronymic": "Patronymic: ",
        "surname": "Surname: ",
        "date_of_birth": "Date of birth: ",
        "category": "Category: ",
        "issuer": "Issuer: "
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
    
    public static func configure(authorizationToken: String, classificationUrl: URL = DocumentFlow.classificationUrl, recognitionUrl: URL = DocumentFlow.recognitionUrl, fileKey: String = "image") -> DriverLicenceFlow {
        return DriverLicenceFlow(type: .driverLicence, authorizationToken: authorizationToken, classificationUrl: classificationUrl, recognitionUrl: recognitionUrl, fileKey: fileKey)
    }
    
    override func defaultTrackingRect() -> CGRect {
        let width = UIScreen.main.bounds.width - 30.0 * 2.0
        let height = width * 0.643
            
        let size = CGSize(width: width, height: height)
        let origin = CGPoint(x: 30.0, y: 200.0)
            
        return CGRect(origin: origin, size: size)
    }
    
    private func title(by key: String) -> String? {
        return super.onReciveResult?(key) ?? keyTitles[key] ?? key
    }
    
}
