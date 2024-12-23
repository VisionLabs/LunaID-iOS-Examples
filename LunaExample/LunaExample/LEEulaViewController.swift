//
//  LEEulaViewController.swift
//  LunaExample
//
//  Created by IVAN CHIRKOV on 17.11.2020.
//

import UIKit
import LunaCamera

class LEEulaViewController: UIViewController {

    private let BetweenOffset: CGFloat = 16
    private let AcceptButtonHeight: CGFloat = 44

    private let textView = UITextView(frame: .zero)
    private let finishButton = LunaCamera.LCRoundButton(type: .custom)

    override func loadView() {
        super.loadView()
        
        createLayout()
    }
    
    private func createLayout() {
        finishButton.translatesAutoresizingMaskIntoConstraints = false
        finishButton.setTitle("accept.button".localized(), for: .normal)
        finishButton.accessibilityIdentifier = "accept_eula"
        finishButton.addTarget(self, action: #selector(accept), for: .touchUpInside)
        finishButton.isEnabled = true
        view.addSubview(finishButton)

        textView.translatesAutoresizingMaskIntoConstraints = false
        textView.textContainerInset.right = 20
        textView.font = Fonts.etelka_13
        textView.text = "EULA".localized()
        view.addSubview(textView)
        
        NSLayoutConstraint.activate([
            textView.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor),
            textView.leadingAnchor.constraint(equalTo: view.leadingAnchor, constant: BetweenOffset),
            textView.trailingAnchor.constraint(equalTo: view.trailingAnchor, constant: -BetweenOffset),
            textView.bottomAnchor.constraint(equalTo: finishButton.topAnchor, constant: -BetweenOffset),

            finishButton.bottomAnchor.constraint(equalTo: view.safeAreaLayoutGuide.bottomAnchor),
            finishButton.leadingAnchor.constraint(equalTo: view.leadingAnchor, constant: BetweenOffset),
            finishButton.trailingAnchor.constraint(equalTo: view.trailingAnchor, constant: -BetweenOffset),
            finishButton.heightAnchor.constraint(equalToConstant: AcceptButtonHeight),
        ])
    }
    
    @objc
    private func accept() {
        UIApplication.acceptEula()
        if UIApplication.isEulaAccepted() {
            dismiss(animated: true, completion: nil)
        }
    }
    
}
