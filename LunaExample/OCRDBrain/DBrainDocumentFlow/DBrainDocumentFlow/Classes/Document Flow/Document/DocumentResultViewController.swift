//
//  DocumentResultViewController.swift
//  dbraion
//
//  Created by Александрк Бельковский on 03/06/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import UIKit

class DocumentResultViewController: UIViewController {
    
    private var mainView: BaseDocumentResultView {
        return view as! BaseDocumentResultView
    }
    
    var respone: RecognitionResponse!
    
    var onEndFlow: (([RecognitionItem]) -> Void)?
    var onReciveResult: ((_ key: String) -> String?)?
    
    // MARK: - Lifecycle
    
    override var prefersStatusBarHidden: Bool {
        return true
    }
    
    override func loadView() {
        view = BaseDocumentResultView()
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        let fields = (respone.items.first?.fields ?? [:]).filter { (key: String, value: RecognitionField) in
            if key == "base64_image" {
                return false
            }
            
            return true
        }
        
        for field in fields {
            
            guard let title = onReciveResult?(field.key) else {
                continue
            }
            
            let view = DocumentItemView()
            view.titleLabel.text = title
            view.valueLabel.text = field.value.text
            
            switch field.value.confidence {
            case 0..<0.4:
                view.confidenceLabel.text = "Low"
                view.confidenceLabel.backgroundColor = UIColor(red: 255.0 / 255.0, green: 48.0 / 255.0, blue: 63.0 / 255.0, alpha: 1.0)
            case 0.4..<0.6:
                view.confidenceLabel.text = "Medium"
                view.confidenceLabel.backgroundColor = UIColor(red: 224.0 / 255.0, green: 139.0 / 255.0, blue: 9.0 / 255.0, alpha: 1.0)
            case 0.6..<1.0:
                view.confidenceLabel.text = "Hight"
                view.confidenceLabel.backgroundColor = UIColor(red: 12.0 / 255.0, green: 198.0 / 255.0, blue: 109.0 / 255.0, alpha: 1.0)
            default:
                break
            }
            
            mainView.stackView.addArrangedSubview(view)
        }

        mainView.retakeButton.addTarget(self, action: #selector(retakeButtonClicked), for: .touchUpInside)
    }
    
    // MARK: - Handler
    
    @objc private func retakeButtonClicked(_ sender: UIButton) {
        onEndFlow?(respone.items)
    }
    
}

