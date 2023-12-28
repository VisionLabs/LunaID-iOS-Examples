//
//  CameraPreviewView.swift
//  Test
//
//  Created by Александрк Бельковский on 19/03/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import UIKit
import AVFoundation

class CameraPreviewView: UIView {
    
    // MARK: - Lifecycle
    
    override class var layerClass: AnyClass {
        return AVCaptureVideoPreviewLayer.self
    }
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        
        backgroundColor = .black
        
        videoPreviewLayer.videoGravity = .resizeAspectFill
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    // MARK: - Vars
    
    var videoPreviewLayer: AVCaptureVideoPreviewLayer! {
        return layer as? AVCaptureVideoPreviewLayer
    }
    
    var session: AVCaptureSession? {
        get {
            return videoPreviewLayer.session
        }
        set {
            videoPreviewLayer.session = newValue
        }
    }
    
}
