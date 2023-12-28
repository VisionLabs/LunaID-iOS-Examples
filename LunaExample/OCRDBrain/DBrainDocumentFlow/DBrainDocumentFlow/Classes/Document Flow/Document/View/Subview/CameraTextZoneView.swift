//
//  CameraTextZoneView.swift
//  Test
//
//  Created by Александрк Бельковский on 03.03.2020.
//  Copyright © 2020 Александрк Бельковский. All rights reserved.
//

import UIKit

class CameraTextZoneView: UIView {
    
    private static let accepZoneCotor: CGColor = UIColor.green.cgColor
    private static let unacceptableZoneColor: CGColor = UIColor.red.cgColor
    
    private var scaleRatio: CGFloat = 8.0
    
    private var previousBounds: CGRect = .zero
    
    private var leftTopAngleLayer: CAShapeLayer = {
        let layer = CAShapeLayer()
        layer.fillColor = UIColor.clear.cgColor
        layer.strokeColor = CameraTextZoneView.unacceptableZoneColor
        layer.lineWidth = 4.5
        layer.strokeEnd = 1.0
        
        return layer
    }()
    
    private var leftBottomAngleLayer: CAShapeLayer = {
        let layer = CAShapeLayer()
        layer.fillColor = UIColor.clear.cgColor
        layer.strokeColor = CameraTextZoneView.unacceptableZoneColor
        layer.lineWidth = 4.5
        layer.strokeEnd = 1.0
        
        return layer
    }()
    
    private var rightTopAngleLayer: CAShapeLayer = {
        let layer = CAShapeLayer()
        layer.fillColor = UIColor.clear.cgColor
        layer.strokeColor = CameraTextZoneView.unacceptableZoneColor
        layer.lineWidth = 4.5
        layer.strokeEnd = 1.0
        
        return layer
    }()
    
    private var rightBottomAngleLayer: CAShapeLayer = {
        let layer = CAShapeLayer()
        layer.fillColor = UIColor.clear.cgColor
        layer.strokeColor = CameraTextZoneView.unacceptableZoneColor
        layer.lineWidth = 4.5
        layer.strokeEnd = 1.0
        
        return layer
    }()
    
    // MARK: - Lifecycle
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        
        layer.addSublayer(leftTopAngleLayer)
        layer.addSublayer(leftBottomAngleLayer)
        layer.addSublayer(rightTopAngleLayer)
        layer.addSublayer(rightBottomAngleLayer)
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    // MARK: - Content
    
    func textZone(apply: Bool) {
        let color = apply ? CameraTextZoneView.accepZoneCotor : CameraTextZoneView.unacceptableZoneColor
        leftTopAngleLayer.strokeColor = color
        leftBottomAngleLayer.strokeColor = color
        rightTopAngleLayer.strokeColor = color
        rightBottomAngleLayer.strokeColor = color
    }
    
    // MARK: - Layout
    
    override func layoutSubviews() {
        super.layoutSubviews()
        
        if previousBounds != bounds {
            
            updateLeftTopAnglePath()
            updateLeftBottomAnglePath()
            updateRightTopAnglePath()
            updateRightBottomAnglePath()
            
            previousBounds = bounds
        }
    }
    
    private func updateLeftTopAnglePath() {
        let path = UIBezierPath()
        
        let minSide = min(bounds.height, bounds.width)
        let part = minSide / scaleRatio
        
        path.move(to: CGPoint(x: 0.0, y: part))
        path.addLine(to: CGPoint(x: 0.0, y: 0.0))
        path.addLine(to: CGPoint(x: part, y: 0.0))
        
        leftTopAngleLayer.path = path.cgPath
    }
    
    private func updateLeftBottomAnglePath() {
        let path = UIBezierPath()
        
        let minSide = min(bounds.height, bounds.width)
        let part = minSide / scaleRatio
        
        path.move(to: CGPoint(x: 0.0, y: bounds.height - part))
        path.addLine(to: CGPoint(x: 0.0, y: bounds.height))
        path.addLine(to: CGPoint(x: part, y: bounds.height))
        
        leftBottomAngleLayer.path = path.cgPath
    }
    
    private func updateRightTopAnglePath() {
        let path = UIBezierPath()
        
        let minSide = min(bounds.height, bounds.width)
        let part = minSide / scaleRatio
        
        path.move(to: CGPoint(x: bounds.width - part, y: 0.0))
        path.addLine(to: CGPoint(x: bounds.width, y: 0))
        path.addLine(to: CGPoint(x: bounds.width, y: part))
        
        rightTopAngleLayer.path = path.cgPath
    }
    
    private func updateRightBottomAnglePath() {
        let path = UIBezierPath()
        
        let minSide = min(bounds.height, bounds.width)
        let part = minSide / scaleRatio
        
        path.move(to: CGPoint(x: bounds.width - part, y: bounds.height))
        path.addLine(to: CGPoint(x: bounds.width, y: bounds.height))
        path.addLine(to: CGPoint(x: bounds.width, y: bounds.height - part))
        
        rightBottomAngleLayer.path = path.cgPath
    }
    
}
