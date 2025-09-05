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
        
        let config = LunaCore.LCLunaConfiguration.userDefaults()
        
        //  To check license only we need to disable primary face functionality in case CNN60 deleted
        config.trackFaceIdentity = false
        
        let lunaIDService: LunaCore.LCLunaIDServiceProtocol = LCLunaIDServiceBuilder.buildLunaIDService(withConfig: config)
        if let error = lunaIDService.activateLicense(with: LCLicenseConfig.userDefaults()) {
            debugPrint("Error while checking license on application startup: \(error)")
        }
        
        let viewController = LERootViewController()
        let navvc = UINavigationController(rootViewController: viewController)
        window = UIWindow(frame: UIScreen.main.bounds)
        window?.backgroundColor = .white
        window?.rootViewController = navvc
        window?.makeKeyAndVisible()
        
        return true
    }
}
