//
//  LEAuthError.swift
//  LunaAuth
//
//  Created by IVAN CHIRKOV on 23.10.2020.
//

import Foundation

/// Error list of the LunaAuth module.
public enum LEAuthError: LocalizedError {
    /// Identification has not been passed.
    case identificationError
    /// Verification has not been passed.
    case verificationError
    /// The person has already been registered in  Luna ID.
    case userAlreadyExists
    /// Verification between best shot and document has not been passed.
    case documentVerificationError
    case faceOnDocumentNotFound
    /// Other errors.
    case error(Error)

    case cancel
    
    /// Localized description of the error.
    public var errorDescription: String? {
        switch self {
        case .identificationError: return "errors.identification_error".localized()
        case .verificationError: return "errors.verification_error".localized()
        case .userAlreadyExists: return "errors.user_already_exists".localized()
        case .documentVerificationError: return "errors.document_verification_error".localized()
        case .faceOnDocumentNotFound: return "errors.face_on_document_not_found".localized()
        case .cancel: return "errors.cancel".localized()
        case .error(let error as LocalizedError):
            return error.errorDescription ?? error.localizedDescription
        case .error(let error):
            return error.localizedDescription
        }
    }
}
