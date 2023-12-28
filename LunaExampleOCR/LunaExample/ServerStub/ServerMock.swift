//
//  ServerStub.swift
//  LunaExample
//
//  Created by IVAN CHIRKOV on 26.10.2020.
//

import Foundation

enum ServerError: LocalizedError {
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

class ServerMock {
    
    enum State<T> {
        case inProgress
        case success(T)
        case failure(Error)
        
        func what() -> String {
            switch self {
            case .failure(let error): return error.what()
            case .inProgress: return "In progress"
            case .success: return "OK"
            }
        }
    }
    
    typealias UserName = String
    typealias UserID = String
    typealias PersonID = String
    typealias LoginCompletion = (State<PersonID>) -> Void
    typealias RegistrationCompletion = LoginCompletion
    
    /// Заглушка аутентификации на клиентском сервере.
    /// - Parameters:
    ///   - userName: Логин.
    ///   - completion: Результат.
    class func login(userName: UserName, completion: @escaping LoginCompletion) {
        completion(.inProgress)
        DispatchQueue.main.asyncAfter(deadline: .now() + Double.random(in: 0.3...1)) {
            let result = Store.personID(by: userName)//Store.userID(by: userName)
            switch result {
            case .success(let personID):
                completion(.success(personID))
            case .failure(let error):
                completion(.failure(error))
            }
        }
    }
    
    class func registration(userName: UserName, completion: @escaping RegistrationCompletion) {
        completion(.inProgress)
        DispatchQueue.main.asyncAfter(deadline: .now() + Double.random(in: 0.3...1)) {
            let result = Store.createUserID(by: userName)
            switch result {
            case .success(let userID):
                completion(.success(userID))
            case .failure(let error):
                completion(.failure(error))
            }
        }
    }
    
    class func savePersonID(_ personID: PersonID, for userID: UserID, completion: @escaping (State<Void>) -> Void) {
        completion(.inProgress)
        DispatchQueue.main.asyncAfter(deadline: .now() + Double.random(in: 0.3...1)) {
            Store.savePersonID(personID, for: userID)
            completion(.success(()))
        }
    }
    
}

private class Store {
    
    typealias UserName = ServerMock.UserName
    typealias UserID = ServerMock.UserID
    typealias PersonID = ServerMock.PersonID
    
    class func userID(by userName: UserName) -> Result<UserID, Error> {
        if let userID = UserDefaults.standard.string(forKey: userName.lowercased()) {
            return .success(userID)
        } else {
            return .failure(ServerError.noUser)
        }
        
    }
    
    class func createUserID(by userName: UserName) -> Result<UserID, Error> {
        let userName = userName.lowercased()
        let personID = self.personID(by: userName)
        switch personID {
        case .success: return .failure(ServerError.userNameAlreadyTaken)
        default: break
        }
        let newUserID = UUID().uuidString
        UserDefaults.standard.setValue(newUserID, forKey: userName)
        return .success(newUserID)
    }
    
    class func personID(by userName: UserName) -> Result<PersonID, Error> {
        switch userID(by: userName) {
        case .success(let userID):
            if let personID = UserDefaults.standard.string(forKey: userID) {
                return .success(personID)
            } else {
                return .failure(ServerError.noPersonID)
            }
        case .failure(let error):
            return .failure(error)
        }
        
    }
    
    class func savePersonID(_ personID: PersonID, for userID: UserID) {
        UserDefaults.standard.setValue(personID, forKey: userID)
    }
    
}
