//
//  ServerStub.swift
//  LunaExample
//
//  Created by IVAN CHIRKOV on 26.10.2020.
//

import Foundation

enum LEServerError: LocalizedError {
    case noUser
    case noPersonID
    case userNameAlreadyTaken
    
    var errorDescription: String? {
        switch self {
        case .noUser: return "server.errors.no_user".localized()
        case .noPersonID: return "server.errors.no_person_id".localized()
        case .userNameAlreadyTaken: return "server.errors.user_name_already_exists".localized()
        }
    }
}
