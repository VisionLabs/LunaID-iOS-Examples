//
//  ActivityIndicator.swift
//  FaceEngineMobileDemo
//
//  Created on 10.11.16.
//  Copyright © 2016 Vision Labs. All rights reserved.
//

import UIKit

class ActivityIndicator: UIImageView {
    
    static let animationKey = "rotationAnimation"
    
    override func awakeFromNib() {
        super.awakeFromNib()
        isHidden = true
        tintColor = Colors.blue
    }
    
    func start() {
        isHidden = false
        let rotationAnimation = CABasicAnimation(keyPath: "transform.rotation")
        rotationAnimation.toValue = CGFloat(Double.pi * 2.0)
        rotationAnimation.duration = 1
        rotationAnimation.repeatCount = Float.infinity
        rotationAnimation.fillMode = CAMediaTimingFillMode.forwards
        layer.add(rotationAnimation, forKey: ActivityIndicator.animationKey)
    }
    
    func stop() {
        isHidden = true
        layer.removeAnimation(forKey: ActivityIndicator.animationKey)
    }

}
