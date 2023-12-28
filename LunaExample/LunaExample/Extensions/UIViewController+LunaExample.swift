//
//  UIViewController+LunaExample.swift
//  LunaExampleOCR
//
//  Created by michael on 25.10.2023.
//

import UIKit

struct LEAlertAction {
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
    
    func presentModalError(_ message: String, actions: [LEAlertAction] = [], needTitle: Bool = true) {
        let alert = UIAlertController(title: needTitle ? "common.error_title".localized() : nil, message: message, preferredStyle: .alert)
        if actions.isEmpty {
            alert.addAction(.init(title: "OK", style: .cancel, handler: nil))
        } else {
            actions.forEach({ action in
                let uiAction = UIAlertAction(title: action.title, style: .default) { (_) in
                    action.handler?()
                }
                alert.addAction(uiAction)
            })
        }
        
        if let target = self.presentedViewController,
           target is UIAlertController {
            print("alert is already presented")
        } else {
            self.present(alert, animated: true, completion: nil)
        }
    }
        
}
