//
//  Data+String.swift
//  dbraion
//
//  Created by Александрк Бельковский on 03/06/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import Foundation

extension Data {
    
    mutating func appendString(_ string: String) {
        guard let data = string.data(using: String.Encoding.utf8, allowLossyConversion: true) else {
            print("Cannot convert string to Data")
            return
        }
        
        append(data)
    }
    
}
