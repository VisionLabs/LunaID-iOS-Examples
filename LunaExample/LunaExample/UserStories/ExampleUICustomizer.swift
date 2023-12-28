//
//  ExampleCustomizer.swift
//  LunaExample
//
//  Created by cipher on 12.02.2023.
//

import UIKit
import LunaCamera

class ExampleUICustomizer: LMUICustomizerProtocol {
    
    func faceDetectionFrameView() -> LMFaceDetectionViewProtocol? {
        return ExampleLMFaceDetectionView(frame: .zero)
    }

    func videoStreamNotificationView() -> LMVideoStreamNotificationViewProtocol? {
        return ExampleNotificationView(frame: .zero)
    }
    
}

class ExampleLMFaceDetectionView: UIView, LMFaceDetectionViewProtocol {
    
    func switchToPositiveState() {
        backgroundColor = UIColor.green.withAlphaComponent(0.3)
    }
    
    func switchToNegativeState() {
        backgroundColor = UIColor.red.withAlphaComponent(0.3)
    }
    
}

class ExampleNotificationView: UIView, LMVideoStreamNotificationViewProtocol {
    
    private let label = UILabel(frame: .zero)
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        
        translatesAutoresizingMaskIntoConstraints = false
        backgroundColor = .cyan

        label.translatesAutoresizingMaskIntoConstraints = false
        label.numberOfLines = 0
        label.backgroundColor = .clear
        addSubview(label)
        
        NSLayoutConstraint.activate([
            label.topAnchor.constraint(equalTo: topAnchor),
            label.bottomAnchor.constraint(equalTo: bottomAnchor),
            label.leadingAnchor.constraint(equalTo: leadingAnchor),
            label.trailingAnchor.constraint(equalTo: trailingAnchor),
        ])
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    func showNotificationMessage(_ newMessage: String) {
        label.text = newMessage
    }
    
    func notificationMessage() -> String? {
        return label.text
    }
    
}
