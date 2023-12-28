//
//  Error+Ext.swift
//  LunaExample
//
//  Created by IVAN CHIRKOV on 27.10.2020.
//

import Foundation

extension Error {
    
    func what() -> String {
        if let error = self as? LocalizedError {
            return error.errorDescription ?? "errors.unknown".localized()
        } else {
            return localizedDescription
        }
    }
}
