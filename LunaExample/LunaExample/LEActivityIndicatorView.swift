//
//  LEActivityIndicatorView.swift
//  LunaExample
//
//  Created by michael on 24.10.2023.
//

import UIKit

class LEActivityIndicatorView: UIView {
    
    private let animationKey = "rotationAnimation"
    private let activityIndicator = UIImageView(image: UIImage(named: "activity_indicator_white"))

    override init(frame: CGRect) {
        super.init(frame: frame)
        
        createLayout()
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    private func createLayout() {
        isHidden = true
        backgroundColor = .black.withAlphaComponent(0.8)
        
        activityIndicator.translatesAutoresizingMaskIntoConstraints = false
        addSubview(activityIndicator)

        NSLayoutConstraint.activate([
            activityIndicator.centerXAnchor.constraint(equalTo: centerXAnchor),
            activityIndicator.centerYAnchor.constraint(equalTo: centerYAnchor)
        ])
    }
    
    func startAnimating() {
        isHidden = false
        let rotationAnimation = CABasicAnimation(keyPath: "transform.rotation")
        rotationAnimation.toValue = CGFloat(Double.pi * 2.0)
        rotationAnimation.duration = 1
        rotationAnimation.repeatCount = Float.infinity
        rotationAnimation.fillMode = CAMediaTimingFillMode.forwards
        activityIndicator.layer.add(rotationAnimation, forKey: animationKey)
    }
    
    func stopAnimating() {
        isHidden = true
        activityIndicator.layer.removeAnimation(forKey: animationKey)
    }
    
}
