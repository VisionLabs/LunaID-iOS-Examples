//
//  LEOCRViewController.swift
//  LunaExampleOCR
//
//  Created by michael on 25.10.2023.
//

import UIKit
import LunaCore
import LunaWeb
import LunaCamera
import OCR

typealias OCRResultHandler = (OCR.OCRResult?) -> Void

class LEOCRViewController: UIViewController, OCRServiceDelegate {
    
    private let SideOffset: CGFloat = 16
    private let ocrInstructionsView = LEOCRInstructionsView(frame: .zero)
    private let activityIndicator = LEActivityIndicatorView(frame: .zero)
    
    private let ocrService = OCRService(ocrAPI: OCRBdrainAPI(delegate: nil))

    public var resultBlock: OCRResultHandler?
    
    override func loadView() {
        super.loadView()
        
        createLayout()
    }
    
    //  MARK: - OCRServiceDelegate -

    func ocrFinishInitialization(error: Error?) {
        if let error = error {
            activityIndicator.stopAnimating()
            presentModalError(error.localizedDescription)
            return
        }
    }
    
    func ocrResult(result: OCR.OCRResult) {
        activityIndicator.stopAnimating()
        resultBlock?(result)
    }
    
    func ocrError(error: Error) {
        activityIndicator.stopAnimating()
        presentModalError(error.localizedDescription)
    }
        
    //  MARK: - Routine -
    
    private func createLayout() {
        view.backgroundColor = .white
        
        ocrInstructionsView.translatesAutoresizingMaskIntoConstraints = false
        ocrInstructionsView.cancelButtonHandler = { [weak self] in
            self?.closeViewController(animated: true)
        }
        ocrInstructionsView.captureIDButtonHandler = { [weak self] in
            guard let self = self else { return }
            self.activityIndicator.startAnimating()
            self.ocrService.showScanner(self, self)
        }
        view.addSubview(ocrInstructionsView)

        activityIndicator.translatesAutoresizingMaskIntoConstraints = false
        view.addSubview(activityIndicator)
        
        NSLayoutConstraint.activate([
            ocrInstructionsView.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: SideOffset),
            ocrInstructionsView.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -SideOffset),
            ocrInstructionsView.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor, constant: SideOffset),
            ocrInstructionsView.bottomAnchor.constraint(equalTo: view.safeAreaLayoutGuide.bottomAnchor, constant: -SideOffset),
            
            activityIndicator.leadingAnchor.constraint(equalTo: view.leadingAnchor),
            activityIndicator.trailingAnchor.constraint(equalTo: view.trailingAnchor),
            activityIndicator.topAnchor.constraint(equalTo: view.topAnchor),
            activityIndicator.bottomAnchor.constraint(equalTo: view.bottomAnchor),
        ])
    }
    
    private func closeViewController(animated: Bool) {
        navigationController?.popViewController(animated: animated)
    }
    
}
