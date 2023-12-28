//
//  UploadFailureViewController.swift
//  Test
//
//  Created by Александрк Бельковский on 20/03/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import UIKit

class UploadFailureViewController: UIViewController {
    
    var errorText: String? = "Sending failed"
    var retakeHandler: (() -> ())?
    
    private var mainView: UploadFailureView {
        return view as! UploadFailureView
    }
    
    // MARK: - Lifecycle
    
    override func loadView() {
        view = UploadFailureView()
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        mainView.errorLabel.text = errorText
        mainView.retakeButton.addTarget(self, action: #selector(retakeButtonClicked), for: .touchUpInside)
    }
    
    // MARK: - Hnadler
    
    @objc private func retakeButtonClicked(_ sender: UIButton) {
        retakeHandler?()
    }
    
}
