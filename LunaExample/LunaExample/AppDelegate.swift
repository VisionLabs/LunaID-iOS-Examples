//
//  AppDelegate.swift
//  LunaExample
//
//  Created by IVAN CHIRKOV on 22.10.2020.
//

import UIKit
import LunaCore

@main
class AppDelegate: UIResponder, UIApplicationDelegate {

    var window: UIWindow?

    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {        
        AppAppearance.setupAppearance()
        
        let configuration = LCLunaConfiguration.defaultConfig()
        configuration.identifyHandlerID = "a43bacae-12ed-4cbf-9926-275a2cf37bfb"
        configuration.registrationHandlerID = "69dcce5a-7507-4442-b700-09dfefcbe226"
        configuration.verifyID = "40fa7644-ea9f-496e-abe0-cf22f1258ac4"
        configuration.lunaAccountID = "12ed7399-f779-479c-8258-bbc45e6017af"
        configuration.lunaServerURL = URL(string: "https://luna-api-aws.visionlabs.ru/6")

        configuration.activateLicense()
        
        let viewController = AuthViewController()
        let navvc = UINavigationController(rootViewController: viewController)
        window = UIWindow(frame: UIScreen.main.bounds)
        window?.backgroundColor = .white
        window?.rootViewController = navvc
        window?.makeKeyAndVisible()
        
        return true
    }

}
