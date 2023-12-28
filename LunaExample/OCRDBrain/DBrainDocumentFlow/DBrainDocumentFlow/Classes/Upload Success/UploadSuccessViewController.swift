//
//  UploadSuccessViewController.swift
//  Test
//
//  Created by Александрк Бельковский on 20/03/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import UIKit

class UploadSuccessViewController: UIViewController {
    
    var doneHandler: (() -> ())?
    
    private var mainView: UploadSuccessView {
        return view as! UploadSuccessView
    }
    
    // MARK: - Lifecycle
    
    override func loadView() {
        view = UploadSuccessView()
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        mainView.doneButton.addTarget(self, action: #selector(doneButtonClicked), for: .touchUpInside)
    }
    
    // MARK: - Hnadler
    
    @objc private func doneButtonClicked(_ sender: UIButton) {
        doneHandler?()
    }
    
}
