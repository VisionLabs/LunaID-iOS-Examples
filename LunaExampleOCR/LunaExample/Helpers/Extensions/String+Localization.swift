//
//  String+Localization.swift
//  FaceEngineMobileDemo
//
//  Created on 25.01.17.
//  Copyright © 2017 Vision Labs. All rights reserved.
//

import Foundation

extension String {
    func localized() -> String {
        return NSLocalizedString(self, tableName: nil, bundle: Bundle.main, value: "", comment: "")
    }
}
