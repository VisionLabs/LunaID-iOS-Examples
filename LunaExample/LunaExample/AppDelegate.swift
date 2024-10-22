//
//  AppDelegate.swift
//  LunaExample
//
//  Created by IVAN CHIRKOV on 22.10.2020.
//

import UIKit
import LunaCore
import LunaWeb

@main
class AppDelegate: UIResponder, UIApplicationDelegate {

    var window: UIWindow?


    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {        
        AppAppearance.setupAppearance()
        
        let viewController = LERootViewController(configuration: LCLunaConfiguration(plist: "luna_config", bundleName: "ai.visionlabs.LunaExample"))
        let navvc = UINavigationController(rootViewController: viewController)
        window = UIWindow(frame: UIScreen.main.bounds)
        window?.backgroundColor = .white
        window?.rootViewController = navvc
        window?.makeKeyAndVisible()
        
        return true
    }
}
