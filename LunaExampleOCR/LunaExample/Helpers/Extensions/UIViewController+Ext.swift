//
//  UIViewController+Ext.swift
//  LunaExample
//
//  Created by IVAN CHIRKOV on 26.10.2020.
//

import UIKit

struct AlertAction {
    let handler: (() -> Void)?
    let title: String
    
    init(title: String) {
        self.title = title
        self.handler = nil
    }
    
    init(title: String, handler: @escaping (() -> Void)) {
        self.title = title
        self.handler = handler
    }
}

extension UIViewController {
    
    func presentError(_ message: String, actions: [AlertAction] = []) {
        let alert = UIAlertController(title: "common.error_title".localized(), message: message, preferredStyle: .alert)
        if actions.count == 0 {
            alert.addAction(.init(title: "OK", style: .cancel, handler: nil))
        } else {
            actions.forEach({ action in
                let uiAction = UIAlertAction(title: action.title, style: .default) { (_) in
                    action.handler?()
                }
                alert.addAction(uiAction)
            })
        }
        DispatchQueue.main.async {
            self.present(alert, animated: true, completion: nil)
        }
    }
}
