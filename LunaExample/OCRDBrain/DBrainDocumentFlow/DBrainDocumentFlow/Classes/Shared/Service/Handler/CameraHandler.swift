//
//  CameraHandler.swift
//  Test
//
//  Created by Александрк Бельковский on 17/04/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import Foundation
import AVFoundation
import UIKit

protocol CameraHandlerDelegate: class {
    
    func cameraHandler(metaRect forCameraHandler: CameraHandler) -> CGRect
    func cameraHandler(devicePosition forCameraHandler: CameraHandler) -> AVCaptureDevice.Position
    func cameraHandler(_ cameraHandler: CameraHandler, didTake asset: AVAsset, url: URL)
}

protocol CameraHandler: AVCaptureVideoDataOutputSampleBufferDelegate {
    
    var delegate: CameraHandlerDelegate? { get set }
    
    func startRecording()
    func stopRecording()
    func cancelRecording()
    
}
