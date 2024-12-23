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
        
        let configuration = LunaCore.LCLunaConfiguration(plist: "halyk_config", bundleName: "ai.visionlabs.LunaExample")
        if let error = configuration.activateLicense() {
            debugPrint("Error while checking license on application startup: \(error)")
        }
        
        let viewController = LERootViewController(configuration: configuration)
        let navvc = UINavigationController(rootViewController: viewController)
        window = UIWindow(frame: UIScreen.main.bounds)
        window?.backgroundColor = .white
        window?.rootViewController = navvc
        window?.makeKeyAndVisible()
        
        return true
    }
}
