//
//  String+Localize.swift
//  LunaCamera
//
//  Created by IVAN CHIRKOV on 29.10.2020.
//

import Foundation

extension String {
    
    func localized() -> String {
        return NSLocalizedString(self, tableName: nil, bundle: Bundle(for: Regula.self), value: "", comment: "")
    }
    
}
