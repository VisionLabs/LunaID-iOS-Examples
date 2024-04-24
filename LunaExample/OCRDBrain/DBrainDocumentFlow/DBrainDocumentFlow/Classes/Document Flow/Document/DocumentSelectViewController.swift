//
//  DocumentSelectViewController.swift
//  dbraion
//
//  Created by Александрк Бельковский on 10/06/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import UIKit

class DocumentSelectViewController: UIViewController {
    
    var values: [ClassificationItem]!
    var loader: DocumentLoader!
    var displayResult: Bool!
    
    var backHandler: (() -> ())?
    var onEndFlow: (([RecognitionItem]) -> Void)?
    var onReciveResult: ((_ key: String) -> String?)?
    
    private var selectedIndex: Int = 0
    
    private var mainView: DocumentSelectView {
        return view as! DocumentSelectView
    }
    
    // MARK: - Lifecycle
    
    override var prefersStatusBarHidden: Bool {
        return false
    }

    override func loadView() {
        view = DocumentSelectView()
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        if let document = values.first?.document {
            let title = document.title ?? document.type
            let isEnabled = document.isEnabled ?? true
            
            mainView.nationalitySelectMenuView.dropDownButton.setTitle(title, for: .normal)
            mainView.nextButton.isEnabled = isEnabled
            mainView.nextButton.alpha = isEnabled ? 1.0 : 0.4
        }
        
        mainView.nationalitySelectMenuView.dropDownButton.addTarget(self, action: #selector(nationalityButtonClicked), for: .touchUpInside)
        mainView.nextButton.addTarget(self, action: #selector(nextButtonClicked), for: .touchUpInside)
        mainView.backButton.addTarget(self, action: #selector(backButtonClicked), for: .touchUpInside)
        let tapGestureRecognizer = UITapGestureRecognizer(target: self, action: #selector(tapViewGestureRecognizer))
        tapGestureRecognizer.delegate = self
        view.addGestureRecognizer(tapGestureRecognizer)
    }
    
    // MARK: - Handler
    
    @objc private func tapViewGestureRecognizer(_ sender: UITapGestureRecognizer) {
        checkAlredyPresented()
    }
    
    @discardableResult private func checkAlredyPresented(type: String? = nil) -> Bool {
        for child in children {
            guard let documentDropDownViewController = child as? DocumentDropDownViewController else {
                break
            }
            
            documentDropDownViewController.willMove(toParent: nil)
            documentDropDownViewController.view.removeFromSuperview()
            documentDropDownViewController.removeFromParent()
            
            if let type = type, documentDropDownViewController.type == type {
                return false
            }
        }
        
        return true
    }
    
    @objc private func backButtonClicked() {
        backHandler?()
    }
    
    @objc private func nationalityButtonClicked(_ sender: UIButton) {
        guard checkAlredyPresented(type: "nationality") else {
            return
        }
        
        let height = CGFloat(min(3, values.count))  * DocumentDropDownCellView.cellHeight()
        let dropDownValues = values.map({ $0.document.title ?? $0.document.type })
        
        let documentDropDownViewController = DocumentDropDownViewController(values: dropDownValues, selectedIndex: selectedIndex)
        documentDropDownViewController.type = "nationality"
        
        documentDropDownViewController.onDidSelect = { [weak self] index in
            guard let self = self, let vc = self.children.first(where: { $0 is DocumentDropDownViewController }) else {
                return
            }
            self.selectedIndex = index
            
            let title = self.values[index].document.title ?? self.values[index].document.type
            let isEnabled = self.values[index].document.isEnabled ?? true
            
            sender.setTitle(title, for: .normal)
            self.mainView.nextButton.isEnabled = isEnabled
            self.mainView.nextButton.alpha = isEnabled ? 1.0 : 0.4
            
            vc.willMove(toParent: nil)
            
            UIView.animate(withDuration: 0.2, animations: {
                vc.view.alpha = 0.0
            }, completion: { success in
                vc.view.removeFromSuperview()
                vc.removeFromParent()
            })
        }
        
        addChild(documentDropDownViewController)
        documentDropDownViewController.willMove(toParent: self)
        
        documentDropDownViewController.view.alpha = 0.0

        mainView.addSubview(documentDropDownViewController.view)
        
        documentDropDownViewController.view.translatesAutoresizingMaskIntoConstraints = false
        documentDropDownViewController.view.heightAnchor.constraint(equalToConstant: height).isActive = true
        documentDropDownViewController.view.leadingAnchor.constraint(equalTo: sender.leadingAnchor).isActive = true
        documentDropDownViewController.view.topAnchor.constraint(equalTo: sender.bottomAnchor).isActive = true
        documentDropDownViewController.view.trailingAnchor.constraint(equalTo: sender.trailingAnchor).isActive = true
        
        documentDropDownViewController.didMove(toParent: self)
        
        UIView.animate(withDuration: 0.2, animations: {
            documentDropDownViewController.view.alpha = 1.0
        })
    }
    
    @objc private func nextButtonClicked(_ sender: LoadingButton) {
        sender.isLoading = true
        
        let value = values[selectedIndex]
        
        guard let image = convertBase64StringToImage(imageBase64String: value.crop) else {
            sender.isLoading = false
            return
        }
        
        loader.recognition(image: image, type: value.document.type) { [weak self] result in
            switch result {
            case .success(let response):
                var updatedResponse: RecognitionResponse = RecognitionResponse(items: response.items,
                                                                               code:response.code,
                                                                               message: response.message, 
                                                                               errno: response.errno)
                if let firstItem: RecognitionItem = updatedResponse.items.first {
                    var mutableFields = firstItem.fields
                    mutableFields["base64_image"] = RecognitionField(text: value.crop, confidence: 1)
                    updatedResponse.items[0] = RecognitionItem(docType: firstItem.docType,
                                                               fields: mutableFields)
                }
                
                if self?.displayResult == true {
                    self?.routeToResult(response: updatedResponse)
                } else {
                    self?.onEndFlow?(response.items)
                }
            case .failure(let error):
                print(error)
            }

            sender.isLoading = false
            
        }
  
    }
    
    private func routeToResult(response: RecognitionResponse) {
        let rocumentResultViewController = DocumentResultViewController()
        rocumentResultViewController.respone = response
        rocumentResultViewController.onReciveResult = onReciveResult
        rocumentResultViewController.onEndFlow = onEndFlow
        
        present(childViewController: rocumentResultViewController) { _ in }
    }
    
    private func convertBase64StringToImage (imageBase64String: String) -> UIImage? {
        if let url = URL(string: imageBase64String) {
            do {
                let imageData = try Data(contentsOf: url)
                let image = UIImage(data: imageData)
                return image
            } catch {
                print(error)
            }
        }
        
        return nil
    }
    
}

// MARK: - UIGestureRecognizerDelegate
extension DocumentSelectViewController: UIGestureRecognizerDelegate {
    
    func gestureRecognizer(_ gestureRecognizer: UIGestureRecognizer, shouldReceive touch: UITouch) -> Bool {
        for child in children {
            guard let documentDropDownViewController = child as? DocumentDropDownViewController else {
                break
            }
            
            if documentDropDownViewController.view.frame.contains(touch.location(in: view)) {
                return false
            }
        }
        
        return true
    }
    
}
