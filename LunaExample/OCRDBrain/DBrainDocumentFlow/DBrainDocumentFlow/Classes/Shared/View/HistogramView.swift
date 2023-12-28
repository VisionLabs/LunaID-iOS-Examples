//
//  HistogramView.swift
//  Test
//
//  Created by Александрк Бельковский on 29/03/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import UIKit

class HistogramView: UIView {

    private let lumaLayer: CAShapeLayer = {
        let layer = CAShapeLayer()
        
        layer.strokeColor = UIColor.gray.cgColor
        layer.fillColor = UIColor.gray.withAlphaComponent(0.8).cgColor
        layer.masksToBounds = true
        layer.lineJoin = CAShapeLayerLineJoin.round
        
        return layer
    }()
    
    private let maxLayer: CAShapeLayer = {
        let layer = CAShapeLayer()
        
        layer.strokeColor = UIColor.red.cgColor
        layer.fillColor = UIColor.red.withAlphaComponent(0.0).cgColor
        layer.masksToBounds = true
        layer.lineJoin = CAShapeLayerLineJoin.round
        
        return layer
    }()
    
    // MARK: - Lifecycle
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        
        backgroundColor = .black
        
        layer.masksToBounds = true
        
        layer.addSublayer(lumaLayer)
        layer.addSublayer(maxLayer)
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    override func layoutSubviews()
    {
        lumaLayer.frame = bounds
        maxLayer.frame = bounds
    }

    // MARK: - Content
    
    func updateLumaLayer(values: [UInt], max: UInt) {
        drawChannel(data: values, maximum: max, layer: lumaLayer)
    }
    
    func updateMaxLayer(borderValue: CGFloat, maximum: UInt) {
        let path = UIBezierPath()
        
        let y = frame.height - (CGFloat(borderValue) / CGFloat(maximum) * frame.height) * 1.0
    
        path.move(to: CGPoint(x: 0.0, y: y))
        path.addLine(to: CGPoint(x: frame.width, y: y))
        
        maxLayer.path = path.cgPath
    }
    
    private func drawChannel(data: [UInt], maximum: UInt, layer: CAShapeLayer)
    {
        let path = UIBezierPath()
        path.move(to: CGPoint(x: 0, y: frame.height))
        
        let interpolationPoints: [CGPoint] = data.enumerated().map
        {
            (index, element) in
            
            let y = frame.height - (CGFloat(element) / CGFloat(maximum) * frame.height) * 1.0
            let x = CGFloat(index) / CGFloat(data.count) * frame.width
            
            return CGPoint(x: x, y: y)
        }
        
        let curves = UIBezierPath()
        curves.interpolatePointsWithHermite(interpolationPoints: interpolationPoints)
        
        path.append(curves)
        
        path.addLine(to: CGPoint(x: frame.width, y: frame.height))
        path.addLine(to: CGPoint(x: 0, y: frame.height))
        
        layer.path = path.cgPath
    }
    
}

private extension UIBezierPath
{
    func interpolatePointsWithHermite(interpolationPoints : [CGPoint], alpha : CGFloat = 1.0/3.0)
    {
        guard !interpolationPoints.isEmpty else { return }
        self.move(to: interpolationPoints[0])
        
        let n = interpolationPoints.count - 1
        
        for index in 0..<n
        {
            var currentPoint = interpolationPoints[index]
            var nextIndex = (index + 1) % interpolationPoints.count
            var prevIndex = index == 0 ? interpolationPoints.count - 1 : index - 1
            var previousPoint = interpolationPoints[prevIndex]
            var nextPoint = interpolationPoints[nextIndex]
            let endPoint = nextPoint
            var mx : CGFloat
            var my : CGFloat
            
            if index > 0
            {
                mx = (nextPoint.x - previousPoint.x) / 2.0
                my = (nextPoint.y - previousPoint.y) / 2.0
            }
            else
            {
                mx = (nextPoint.x - currentPoint.x) / 2.0
                my = (nextPoint.y - currentPoint.y) / 2.0
            }
            
            let controlPoint1 = CGPoint(x: currentPoint.x + mx * alpha, y: currentPoint.y + my * alpha)
            currentPoint = interpolationPoints[nextIndex]
            nextIndex = (nextIndex + 1) % interpolationPoints.count
            prevIndex = index
            previousPoint = interpolationPoints[prevIndex]
            nextPoint = interpolationPoints[nextIndex]
            
            if index < n - 1
            {
                mx = (nextPoint.x - previousPoint.x) / 2.0
                my = (nextPoint.y - previousPoint.y) / 2.0
            }
            else
            {
                mx = (currentPoint.x - previousPoint.x) / 2.0
                my = (currentPoint.y - previousPoint.y) / 2.0
            }
            
            let controlPoint2 = CGPoint(x: currentPoint.x - mx * alpha, y: currentPoint.y - my * alpha)
            
            self.addCurve(to: endPoint, controlPoint1: controlPoint1, controlPoint2: controlPoint2)
        }
    }
}
