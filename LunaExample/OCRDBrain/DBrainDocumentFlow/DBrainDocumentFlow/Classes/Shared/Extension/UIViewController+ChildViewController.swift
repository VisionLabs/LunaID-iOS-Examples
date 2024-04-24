//
//  UIViewController+ChildViewController.swift
//  dbraion
//
//  Created by Александрк Бельковский on 31/05/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import UIKit

extension UIViewController {
    
    func present(childViewController: UIViewController, completion: @escaping (Bool) -> Void) {
        addChild(childViewController)
        childViewController.willMove(toParent: self)
        
        childViewController.view.alpha = 0.0
        add(childView: childViewController.view, in: view)
        childViewController.didMove(toParent: self)
        
        UIView.animate(withDuration: 0.2, animations: {
            childViewController.view.alpha = 1.0
        }, completion: completion)
    }
    
    func remove(childViewController: UIViewController, completion: ((Bool) -> Void)? = nil) {
        childViewController.willMove(toParent: nil)
        
        UIView.animate(withDuration: 0.2, animations: {
            childViewController.view.alpha = 0.0
        }, completion: { [weak childViewController] success in
            childViewController?.view.removeFromSuperview()
            childViewController?.removeFromParent()
            completion?(success)
        })
    }
    
    private func add(childView: UIView, in view: UIView) {
        childView.translatesAutoresizingMaskIntoConstraints = false
        view.addSubview(childView)
        childView.leftAnchor.constraint(equalTo: view.leftAnchor).isActive = true
        childView.topAnchor.constraint(equalTo: view.topAnchor).isActive = true
        childView.trailingAnchor.constraint(equalTo: view.trailingAnchor).isActive = true
        childView.bottomAnchor.constraint(equalTo: view.bottomAnchor).isActive = true
    }
    
}
