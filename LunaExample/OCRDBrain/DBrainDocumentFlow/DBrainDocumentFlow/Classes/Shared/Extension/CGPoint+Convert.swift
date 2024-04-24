//
//  CGPoint+Convert.swift
//  Test
//
//  Created by Александрк Бельковский on 19/03/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import UIKit

extension CGPoint {
    
    func convert(_ size: CGSize, _ cameraPreviewView: CameraPreviewView) -> CGPoint {
        
        let point = CGPoint(x: self.x / size.width,
                            y: self.y / size.height)
        
        return cameraPreviewView.videoPreviewLayer.layerPointConverted(fromCaptureDevicePoint: point)
    }
    
}
