//
//  CameraTextHandler.swift
//  Test
//
//  Created by Александрк Бельковский on 18/04/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import Foundation
import AVFoundation

protocol CameraTextHandlerDelegate: class {
    func cameraTextHandler(_ cameraTextHandler: CameraTextHandler, reciveLuma histogram: [UInt], maximum: UInt, minimum: UInt)
}

class CameraTextHandler: NSObject, CameraHandler {

    weak var textDelegate: CameraTextHandlerDelegate?
    weak var delegate: CameraHandlerDelegate?
    
    private var isProcessing: Bool = false
    
    private let cameraUtils: CameraUtils = CameraUtils()

    func startRecording() {
        
    }
    
    func stopRecording() {
        
    }
    
    func cancelRecording() {
        
    }
    
}

// MARK: - AVCaptureVideoDataOutputSampleBufferDelegate
extension CameraTextHandler {
    
    func captureOutput(_ output: AVCaptureOutput, didOutput sampleBuffer: CMSampleBuffer, from connection: AVCaptureConnection) {
        guard let delegate = delegate else {
            return
        }
        
        guard let image = cameraUtils.image(from: sampleBuffer) else {
            return
        }
        
        let metaRect = delegate.cameraHandler(metaRect: self)
        guard let croppedImage = cameraUtils.crop(image: image, to: metaRect), let croppedCGImage = croppedImage.cgImage else {
            return
        }
        
        if let lumaValues = cameraUtils.lumaHistogram(from: croppedCGImage) {
            let maximumLumaValue = lumaValues.max() ?? 0
            let minimumLumaValue = lumaValues.min() ?? 0
            DispatchQueue.main.async { [weak self] in
                guard let self = self else {
                    return
                }
                
                self.textDelegate?.cameraTextHandler(self, reciveLuma: lumaValues,
                                                     maximum: maximumLumaValue,
                                                     minimum: minimumLumaValue)
            }
        }
        
    }
    
}
