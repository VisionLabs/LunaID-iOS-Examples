//
//  DocumentDropDownButtonView.swift
//  dbraion
//
//  Created by Александрк Бельковский on 10/06/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import UIKit

class DocumentDropDownButtonView: UIButton {
    
    private var previousBounds: CGRect = .zero
    
    private lazy var iconShapeLayer: CAShapeLayer = {
        let layer = CAShapeLayer()
        layer.fillColor = UIColor(red: 250.0 / 255.0, green: 250.0 / 255.0, blue: 250.0 / 255.0, alpha: 1.0).cgColor
        layer.strokeColor = UIColor(red: 250.0 / 255.0, green: 250.0 / 255.0, blue: 250.0 / 255.0, alpha: 1.0).cgColor
        layer.lineWidth = 2.0
        //layer.lineCap = .round
        self.layer.addSublayer(layer)
        
        return layer
    }()
    
    // MARK: - Layout
    
    override func layoutSubviews() {
        super.layoutSubviews()
        
        if previousBounds != bounds {
            updateIconPath()
            previousBounds = bounds
        }
    }
    
    private func updateIconPath() {
        let path = UIBezierPath()
        
        let startX = bounds.maxX - 24.0 - 16.0
        let startY = bounds.midY - 4.0
        let start = CGPoint(x: startX, y: startY)
        
        let middleX = bounds.maxX - 24.0 - 8.0
        let middleY = bounds.midY + 4.0
        let middle = CGPoint(x: middleX, y: middleY)
        
        let endX = bounds.maxX - 24.0
        let endY = bounds.midY - 4.0
        let end = CGPoint(x: endX, y: endY)
        
        path.move(to: start)
        path.addLine(to: middle)
        path.addLine(to: end)
        path.close()
        
        iconShapeLayer.path = path.cgPath
    }
    
}
