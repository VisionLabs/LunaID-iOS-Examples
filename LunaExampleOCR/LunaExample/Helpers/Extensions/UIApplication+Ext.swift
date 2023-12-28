//
//  UIApplication+Ext.swift
//  LunaMobile
//
//  Created by cipher on 16.03.2023.
//

import UIKit

private let eulaKey = "isEulaAccepted"

extension UIApplication {
    
    static func openAppSettings() {
        guard let settingsUrl = URL(string: UIApplication.openSettingsURLString) else {
            return
        }
        if UIApplication.shared.canOpenURL(settingsUrl) {
            UIApplication.shared.open(settingsUrl)
        }
    }
    
    static func isEulaAccepted() -> Bool {
        return UserDefaults.standard.bool(forKey: eulaKey)
    }
    
    static func acceptEula() {
        UserDefaults.standard.set(true, forKey: eulaKey)
    }
    
}
