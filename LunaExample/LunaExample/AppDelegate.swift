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
        
        if #available(iOS 13.0, *) {
            // На iOS 13+ пилим сцену
        } else {
            let viewController = LERootViewController()
            let navvc = UINavigationController(rootViewController: viewController)
            window = UIWindow(frame: UIScreen.main.bounds)
            window?.backgroundColor = .white
            window?.rootViewController = navvc
            window?.makeKeyAndVisible()
        }
        
        return true
    }
    
    @available(iOS 13.0, *)
    func application(
        _ application: UIApplication,
        configurationForConnecting connectingSceneSession: UISceneSession,
        options: UIScene.ConnectionOptions
    ) -> UISceneConfiguration {
        UISceneConfiguration(
            name: "DefaultConfiguration",
            sessionRole: connectingSceneSession.role
        )
    }

    @available(iOS 13.0, *)
    func application(_ application: UIApplication,
                     didDiscardSceneSessions sceneSessions: Set<UISceneSession>) {
    }
}
