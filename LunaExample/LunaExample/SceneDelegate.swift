//
//  SceneDelegate.swift
//  LunaMobile
//
//  Created by  Геращенко Андрей on 13.11.2025.
//

import UIKit

@available(iOS 13.0, *)
class SceneDelegate: UIResponder, UIWindowSceneDelegate {

    var window: UIWindow?

    func scene(
        _ scene: UIScene,
        willConnectTo session: UISceneSession,
        options connectionOptions: UIScene.ConnectionOptions
    ) {
        guard let windowScene = (scene as? UIWindowScene) else { return }
        let viewController = LERootViewController()
        let navvc = UINavigationController(rootViewController: viewController)
        let newWindow = UIWindow(windowScene: windowScene)
        newWindow.backgroundColor = .white
        newWindow.rootViewController = navvc
        newWindow.makeKeyAndVisible()
        self.window = newWindow
    }

    func sceneDidBecomeActive(_ scene: UIScene) {}
    func sceneWillResignActive(_ scene: UIScene) {}
    func sceneWillEnterForeground(_ scene: UIScene) {}
    func sceneDidEnterBackground(_ scene: UIScene) {}
}
