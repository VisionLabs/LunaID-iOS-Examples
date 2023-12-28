//
//  EulaViewController.swift
//  LunaExample
//
//  Created by IVAN CHIRKOV on 17.11.2020.
//

import UIKit

class EulaViewController: UIViewController {

    @IBOutlet weak var textViewWidthConstraint: NSLayoutConstraint!
    @IBOutlet weak var textViewHeightConstraint: NSLayoutConstraint!
    
    @IBOutlet private weak var textView: UITextView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        textView.textContainerInset.right = 20
    }
    
    @IBAction func accept() {
        UIApplication.acceptEula()
        if UIApplication.isEulaAccepted() {
            dismiss(animated: true, completion: nil)
        }
    }
    
    override func updateViewConstraints() {
        textViewWidthConstraint.constant = view.frame.width - 14
        textViewHeightConstraint.constant = view.frame.height - 83
        super.updateViewConstraints()
    }
    
    
    var isScrollFixed = false
    
    override func viewDidLayoutSubviews() {
        super.viewDidLayoutSubviews()
        if !isScrollFixed {
            textView.setContentOffset(.zero, animated: false)
            isScrollFixed = true
        }
    }

}
