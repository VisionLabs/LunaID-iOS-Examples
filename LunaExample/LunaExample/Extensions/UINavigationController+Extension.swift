//
//  UINavigationController+Extension.swift
//  LunaExample
//
//  Created by Kirill Zhadaev on 18.04.2024.
//

import UIKit

extension UINavigationController {
    func popViewController(animated: Bool = true, completion: VoidHandler? = nil) {
        popViewController(animated: animated)
        if let transitionCoordinator {
            transitionCoordinator.animate(alongsideTransition: nil) { _ in
                completion?()
            }
        } else {
            completion?()
        }
    }
}
