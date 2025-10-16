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
        let license: LCLicenseConfig
        if LCLicenseConfig.userDefaults().eid.isEmpty || LCLicenseConfig.userDefaults().productID.isEmpty ||
            LCLicenseConfig.userDefaults().eid == "00d2c76a-edae-447f-b074-63636aad42e6" ||
            LCLicenseConfig.userDefaults().productID == "28a5e16a-51d6-40fb-bcdc-b5815a70849b" {
            let licensePath = Bundle.main.path(forResource: config.plistLicenseFileName, ofType: nil) ?? ""
            license = LCLicenseConfig(plistFilePath: licensePath)
            license.save()
        } else {
            license = LCLicenseConfig.userDefaults()
        }
        if let error = lunaIDService.activateLicense(with: license) {
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
