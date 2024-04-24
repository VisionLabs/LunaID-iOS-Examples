//
//  Error.swift
//  OCRREGULA
//
//  Created by IVAN CHIRKOV on 10.03.2021.
//

import Foundation

public enum Error: LocalizedError {
    case noLicense
    case unknown
    
    /// :nodoc:
    public var errorDescription: String? {
        switch self {
        case .noLicense: return "errors.noLicense".localized()
        case .unknown: return "errors.unknown".localized()
        }
    }
}
