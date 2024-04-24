//
//  String+Validation.swift
//  FaceEngineMobileDemo
//
//  Created on 02.11.16.
//  Copyright © 2016 Vision Labs. All rights reserved.
//

import Foundation

enum ValidationError: LocalizedError {
    case minChar(Int)
    case maxChar(Int)
    case invalidCharacters
    
    var errorDescription: String? {
        switch self {
        case .minChar(let count): return "validation.errors.min_characters".localized() + "\(count)"
        case .maxChar(let count): return "validation.errors.max_characters".localized() + "\(count)"
        case .invalidCharacters: return "validation.errors.invalid_characters".localized()
        }
    }
}

extension String {
    
    /**
        Проверяет на валидность строку как email.
    */
    var isValidAsEmail: Bool {
        guard !self.isEmpty else {
            return false
        }
        let emailRegEx = "[A-Z0-9a-z.-_]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,6}"
        let regex = try! NSRegularExpression(pattern: emailRegEx)
        let originalRange = NSRange(location: 0, length: self.count)
        let matchedRange = regex.rangeOfFirstMatch(in: self, options: [], range: originalRange)
        return originalRange.length == matchedRange.length
    }
    
    /**
        Проверяет на валидность строку как имя.
    */
    func isValidAsName() throws -> Bool {
        guard !self.isEmpty, self.count > 2 else {
            throw ValidationError.minChar(3)
        }
        if count > 128 { throw ValidationError.maxChar(128) }
        let nameRegEx = "[A-Za-zА-Яа-я]{1}+[A-Za-zА-Яа-я0-9]{2,127}"
        let regex = try! NSRegularExpression(pattern: nameRegEx)
        let originalRange = NSRange(location: 0, length: self.count)
        let matchedRange = regex.rangeOfFirstMatch(in: self, options: [], range: originalRange)
        guard originalRange.length == matchedRange.length else {
            throw ValidationError.invalidCharacters
        }
        return true
    }
    
    var isValidAsPhoneNumber: Bool {
        guard !self.isEmpty else {
            return false
        }
        
        let phoneRegEx = "[+]{0,1}+[0-9]{1,}"
        let regex = try! NSRegularExpression(pattern: phoneRegEx)
        let originalRange = NSRange(location: 0, length: self.count)
        let matchedRange = regex.rangeOfFirstMatch(in: self, options: [], range: originalRange)
        return originalRange.length == matchedRange.length
    }
}

extension Optional where Wrapped == String {
    
    var isValidAsEmail: Bool {
        return self?.isValidAsEmail ?? false
    }
    
    func isValidAsName() throws -> Bool {
        switch self {
        case .some(let value): return try value.isValidAsName()
        case .none: throw ValidationError.minChar(3)
        }
    }
    
    var isValidAsPhoneNumber: Bool {
        return self?.isValidAsPhoneNumber ?? false
    }
    
}
