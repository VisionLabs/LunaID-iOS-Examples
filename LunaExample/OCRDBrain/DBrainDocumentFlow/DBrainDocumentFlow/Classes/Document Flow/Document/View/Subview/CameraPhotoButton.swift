//
//  CameraPhotoButton.swift
//  Test
//
//  Created by Александрк Бельковский on 03.03.2020.
//  Copyright © 2020 Александрк Бельковский. All rights reserved.
//

import UIKit

class CameraPhotoButton: LoadingButton {
    
    private var previousBounds: CGRect = .zero
    
    private var ovalLayer: CAShapeLayer = {
        let layer = CAShapeLayer()
        layer.fillColor = UIColor.clear.cgColor
        layer.strokeColor = UIColor.red.cgColor
        layer.lineWidth = 3.0
        layer.strokeEnd = 1.0
        
        return layer
    }()
    
    // MARK: - Lifecycle
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        
        backgroundColor = UIColor(red: 221.0 / 255.0, green: 221.0 / 255.0, blue: 221.0 / 255.0, alpha: 1.0)
        isEnabled = false
        
        layer.masksToBounds = true
        layer.addSublayer(ovalLayer)
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        super.touchesBegan(touches, with: event)
        
        UIView.animate(withDuration: 0.2) {
            self.alpha = 0.4
        }
    }
    
    override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent?) {
        super.touchesEnded(touches, with: event)
        
        UIView.animate(withDuration: 0.2) {
            self.alpha = 1.0
        }
    }
    
    // MARK: - Content
    
    func button(apply: Bool) {
        isEnabled = apply
        ovalLayer.strokeColor = apply ? UIColor.white.cgColor : UIColor(red: 198.0 / 255.0, green: 12.0 / 255.0, blue: 49.0 / 255.0, alpha: 1.0).cgColor
        backgroundColor = apply ? UIColor(red: 12.0 / 255.0, green: 198.9 / 255.0, blue: 109.0 / 255.0, alpha: 1.0) : UIColor(red: 221.0 / 255.0, green: 221.0 / 255.0, blue: 221.0 / 255.0, alpha: 1.0)
    }
    
    // MARK: - Layout
    
    override func layoutSubviews() {
        super.layoutSubviews()
        
        if previousBounds != bounds {
            let rect = bounds.inset(by: UIEdgeInsets(top: 6.0, left: 6.0, bottom: 6.0, right: 6.0))
            ovalLayer.path = UIBezierPath(ovalIn: rect).cgPath
            
            layer.cornerRadius = bounds.width / 2.0
            
            previousBounds = bounds
        }
    }
    
}
