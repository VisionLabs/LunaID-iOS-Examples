//
//  BaseDocumentView.swift
//  dbraion
//
//  Created by Александрк Бельковский on 08/06/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import UIKit

class BaseDocumentView: UIView {
    
    private var trackingRect: CGRect
    
    var cameraPreviewView: CameraPreviewView = {
        let view = CameraPreviewView()
        view.translatesAutoresizingMaskIntoConstraints = false
        
        return view
    }()
    
    var photoButton: CameraPhotoButton = {
        let button = CameraPhotoButton(type: .system)
        button.tintColor = UIColor.white
        button.button(apply: false)
        button.translatesAutoresizingMaskIntoConstraints = false

        return button
    }()
    
    private var infoLabel: UILabel = {
        let label = UILabel()
        label.textAlignment = .center
        label.translatesAutoresizingMaskIntoConstraints = false
        
        label.alpha = 0.0
        
        return label
    }()
    
    // MARK: - Debug views
    
    lazy var histogramView: HistogramView = {
        let view = HistogramView()
        view.backgroundColor = UIColor.black.withAlphaComponent(0.6)
        
        view.translatesAutoresizingMaskIntoConstraints = false
        view.isUserInteractionEnabled = false
        
        return view
    }()
    
    // MARK: - Lifecycle
    
    init(trackingRect: CGRect) {
        self.trackingRect = trackingRect
        
        super.init(frame: .zero)
        
        backgroundColor = .black
        
        addSubview(cameraPreviewView)
        
        cameraPreviewView.addSubview(photoButton)
        cameraPreviewView.addSubview(infoLabel)
        
        setupConstraints()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    // MARK: - Layout
    
    private func setupConstraints() {
        cameraPreviewView.leftAnchor.constraint(equalTo: leftAnchor).isActive = true
        cameraPreviewView.topAnchor.constraint(equalTo: topAnchor).isActive = true
        cameraPreviewView.trailingAnchor.constraint(equalTo: trailingAnchor).isActive = true
        cameraPreviewView.bottomAnchor.constraint(equalTo: bottomAnchor).isActive = true
        
        infoLabel.centerXAnchor.constraint(equalTo: centerXAnchor).isActive = true
        infoLabel.bottomAnchor.constraint(equalTo: photoButton.topAnchor, constant: -39.0).isActive = true
        
        photoButton.centerXAnchor.constraint(equalTo: centerXAnchor).isActive = true
        photoButton.heightAnchor.constraint(equalToConstant: 72.0).isActive = true
        photoButton.widthAnchor.constraint(equalToConstant: 72.0).isActive = true
        
        if #available(iOS 11.0, *) {
            photoButton.bottomAnchor.constraint(equalTo: safeAreaLayoutGuide.bottomAnchor, constant: -24.0).isActive = true
        } else {
            photoButton.bottomAnchor.constraint(equalTo: bottomAnchor, constant: -24.0).isActive = true
        }
    }
    
    // MARK: - Content
    
    func setupInfo(text: String, color: UIColor, font: UIFont) {
        let attributedString = NSMutableAttributedString(string: text)
        let range = NSRange(location: 0, length: attributedString.length)
        attributedString.addAttribute(.kern, value: -0.32, range: range)
        attributedString.addAttribute(.font, value: font, range: range)
        attributedString.addAttribute(.foregroundColor, value: color, range: range)
        
        infoLabel.attributedText = attributedString
    }
    
    func info(isHidden: Bool) {
        let alpha: CGFloat = isHidden ? 0.0 : 1.0
        
        guard infoLabel.alpha != alpha else {
            return
        }
        
        
        UIView.animate(withDuration: 0.2) {
            self.infoLabel.alpha = alpha
        }
    }
    
    func view(apply: Bool) {
        photoButton.button(apply: apply)
    }
    
}

