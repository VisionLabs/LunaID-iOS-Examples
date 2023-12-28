//
//  AppAppearance.swift
//  FaceEngineMobileDemo
//
//  Created on 07.11.16.
//  Copyright © 2016 Vision Labs. All rights reserved.
//

import UIKit

class AppAppearance {
    
    static let detectionAreaValidColor = UIColor.green
    static let detectionAreaInvalidColor = Colors.red
    
    class func setupAppearance() {
        UINavigationBar.appearance().tintColor = Colors.blue
        UINavigationBar.appearance().titleTextAttributes = [
            .font: UIFont.systemFont(ofSize: 17, weight: .medium)
        ]
        UIBarButtonItem.appearance().setTitleTextAttributes(
            [.font: UIFont.systemFont(ofSize: 17)],
            for: [.normal])
        
        UIBarButtonItem.appearance().setTitleTextAttributes(
            [.font: UIFont.systemFont(ofSize: 17)],
            for: [.highlighted])
        
        UIBarButtonItem.appearance().setTitleTextAttributes(
            [.font: UIFont.systemFont(ofSize: 17)],
            for: [.disabled])
    }
    
}
