//
// Created by IVAN CHIRKOV on 21.04.2021.
//

import Foundation

public enum OCRError: LocalizedError {
    /// Operation canceled.
    case canceled
    case notRecognized
    case photoNotAccepted
    /// :nodoc:
    case noDocumentType
    /// OCR internal error.
    case error(Error)

    /// :nodoc:
    public var errorDescription: String? {
        switch self {
        case .canceled: return "errors.canceled".localized()
        case .notRecognized: return "errors.notRecognized".localized()
        case .photoNotAccepted: return "errors.photoNotAccepted".localized()
        case .noDocumentType: return "errors.noDocumentType".localized()
        case .error(let error as LocalizedError):
            return error.errorDescription ?? error.localizedDescription
        case .error(let error):
            return error.localizedDescription
        }
    }
}
