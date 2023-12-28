//
//  CameraService.swift
//  Test
//
//  Created by Александрк Бельковский on 18/03/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import Foundation
import AVFoundation
import UIKit

enum CameraConstans {
    static let redCoefficient: Float = 0.2126 // 0.299
    static let greenCoefficient: Float = 0.7152 // 0.587
    static let blueCoefficient: Float = 0.0722 // 0.114
}

private enum CameraConfiguration {
    case preparation, success, fail
}

protocol CameraServiceDelegate: class {
    
    func cameraService(_ cameraService: CameraService, didTake image: UIImage)
    func cameraService(_ cameraService: CameraService, didTake asset: AVAsset, url: URL)
    
    func cameraService(targetRect cameraService: CameraService) -> CGRect
    
    func cameraService(didStart cameraService: CameraService)
    func cameraService(didStop cameraService: CameraService)
}

class CameraService: NSObject {

    weak var delegate: CameraServiceDelegate?
    
    var cameraHandler: CameraHandler?
    
    private var position: AVCaptureDevice.Position
    
    private var queue: DispatchQueue = DispatchQueue(label: "com.camera.queue")
    private var delegateQueue: DispatchQueue = DispatchQueue(label: "com.output.camera.queue")
    
    private var session: AVCaptureSession = AVCaptureSession()
    
    private var configuration: CameraConfiguration = .preparation
    private var cameraPreviewView: CameraPreviewView

    private let photoOutput: AVCaptureStillImageOutput = {
        let output = AVCaptureStillImageOutput()
        output.outputSettings = [AVVideoCodecKey: AVVideoCodecJPEG]
        
        return output
    }()
    
    private let output: AVCaptureVideoDataOutput = {
        let output = AVCaptureVideoDataOutput()
        
        return output
    }()
    
    // MARK: - Lifecycle
    
    init(with cameraPreviewView: CameraPreviewView, cameraPosition position: AVCaptureDevice.Position) {
        self.cameraPreviewView = cameraPreviewView
        cameraPreviewView.session = self.session
        
        self.position = position
        
        super.init()
    }
    
    // MARK: - Implementation
    
    func start() {
        if configuration == .success, !session.isRunning {
            cameraHandler?.delegate = self
            
            output.setSampleBufferDelegate(cameraHandler, queue: delegateQueue)
            session.startRunning()
            didStart()
        } else {
            queue.async { [weak self] in
                self?.configure { [weak self] success in
                    guard success else {
                        return
                    }
                    self?.session.startRunning()
                    self?.didStart()
                }
            }
        }
    }
    
    func stop() {
        stopRecording()
        
        queue.async { [weak self] in
            guard let `self` = self else {
                return
            }
            
            if self.session.isRunning {
                self.session.stopRunning()
                self.didStop()
            }
        }
    }
    
    private func didStart() {
        DispatchQueue.main.async { [weak self] in
            guard let self = self else {
                return
            }
            
            self.delegate?.cameraService(didStart: self)
        }
    }
    
    private func didStop() {
        DispatchQueue.main.async { [weak self] in
            guard let self = self else {
                return
            }
            
            self.delegate?.cameraService(didStop: self)
        }
    }

    func takePhoto() {
        guard let connection = photoOutput.connection(with: .video) else {
            return
        }
        
//        if connection.isVideoMirroringSupported {
//            connection.isVideoMirrored = false
//        }
        
        let layerRect = cameraPreviewView.frame
        
        readBuffer(form: connection) { [weak self] buffer in
            guard let `self` = self, let buffer = buffer else { return }
            self.createImage(layerRect, from: buffer)
        }
    }
    
    func tempURL() -> URL {
        let directory = NSTemporaryDirectory() as NSString
        
        let path = directory.appendingPathComponent(NSUUID().uuidString + ".mp4")
        return URL(fileURLWithPath: path)
    }
    
    func startRecording() {
        cameraHandler?.startRecording()
    }
    
    func stopRecording() {
        cameraHandler?.stopRecording()
    }
    
    func cancelRecording() {
        cameraHandler?.cancelRecording()
    }
    
    func requestAccess(completion: @escaping (Bool) -> Void) {
        if needAccess() {
            completion(true)
        } else {
            AVCaptureDevice.requestAccess(for: .video, completionHandler: completion)
        }
    }
    
    // MARK: - Private implementation
    
    private func readBuffer(form connection: AVCaptureConnection, completion: @escaping (CMSampleBuffer?) -> Void) {
        guard !photoOutput.isCapturingStillImage else { return }
        
        photoOutput.captureStillImageAsynchronously(from: connection) { buffer, _ in
            completion(buffer)
        }
    }
    
    private func createImage(_ rect: CGRect, from buffer: CMSampleBuffer) {
        DispatchQueue.global(qos: .userInitiated).async { [weak self] in
            guard let `self` = self,
                let data = AVCaptureStillImageOutput.jpegStillImageNSDataRepresentation(buffer),
                let image = self.processPhoto(rect, from: data) else {
                    return
            }
            DispatchQueue.main.async {
                self.delegate?.cameraService(self, didTake: image)
            }
        }
        
    }
    
    private func processPhoto(_ rect: CGRect, from data: Data) -> UIImage? {
        guard let provider = CGDataProvider(data: data as CFData),
            let cgImage = CGImage(jpegDataProviderSource: provider,
                                  decode: nil, shouldInterpolate: true,
                                  intent: .defaultIntent) else {
                                    return nil
        }
        
        let orientation: UIImage.Orientation = position == .back ? .right : .leftMirrored
        
        let image = UIImage(cgImage: cgImage, scale: 1.0, orientation: orientation)
        return cropTo(rect, from: image)
    }
    
    private func cropTo(_ rect: CGRect, from image: UIImage?) -> UIImage? {
        guard let image = image else { return nil }
        
        let originalSize = CGSize(width: image.size.height, height: image.size.width)
        
        let metaRect = CGRect(x: rect.origin.y / UIScreen.main.bounds.height,
                              y: rect.origin.x / UIScreen.main.bounds.width,
                              width: rect.height / UIScreen.main.bounds.height,
                              height: rect.width / UIScreen.main.bounds.width)

        let cropRect = CGRect(x: metaRect.origin.x * originalSize.width,
                              y: metaRect.origin.y * originalSize.height,
                              width: metaRect.size.width * originalSize.width,
                              height: metaRect.size.height * originalSize.height).integral
        
        guard let cgImage = image.cgImage, let cropImage = cgImage.cropping(to: cropRect) else { return nil }
        
        let finalImage = UIImage(cgImage: cropImage, scale: 1.0, orientation: image.imageOrientation)
        
        return finalImage
    }
    
    private func needAccess() -> Bool {
        return AVCaptureDevice.authorizationStatus(for: .video) == .authorized
    }
    
    private func getCurrentDevice() -> AVCaptureDevice? {
        if #available(iOS 10.0, *) {
            return AVCaptureDevice.default(.builtInWideAngleCamera, for: .video, position: position)
        } else {
            let devices = AVCaptureDevice.devices(for: .video)
            return devices.first(where: { $0.position == position })
        }
    }
    
    private func configure(completion: @escaping (Bool) -> Void) {
        configuration = .success
        
        session.beginConfiguration()
        
        configurePreset()
        configureInput()
        configureOutput()
        
        session.commitConfiguration()
        
        if let connection = photoOutput.connection(with: .video) {
            if connection.isVideoMirroringSupported {
                connection.isVideoMirrored = false
            }
        }
        
        if let connection = output.connection(with: .video) {
            if connection.isVideoMirroringSupported {
                connection.isVideoMirrored = false
            }
            
            //connection.videoOrientation = .portrait
        }
        
        completion(configuration == .success)
    }
    
    private func configurePreset() {
        session.sessionPreset = .high
    }
    
    private func configureInput() {
        if let device = getCurrentDevice() {
            
            configure(device: device)
            
            do {
                let input = try AVCaptureDeviceInput(device: device)
                
                if session.canAddInput(input) {
                    session.addInput(input)
                } else {
                    configuration = .fail
                    print("Can add input to session")
                }
                
            } catch {
                configuration = .fail
                print("Can create video input from device")
            }
            
        } else {
            configuration = .fail
            print("Can get device")
        }
        
    }
    
    private func configureOutput() {
        cameraHandler?.delegate = self
        
        output.setSampleBufferDelegate(self.cameraHandler, queue: self.delegateQueue)
        
        if session.canAddOutput(output) && session.canAddOutput(photoOutput) {
            session.addOutput(photoOutput)
            session.addOutput(output)
        } else {
            configuration = .fail
            print("Can add output to session")
        }
    }
    
    private func configure(device: AVCaptureDevice) {
        do {
            try device.lockForConfiguration()
            
            device.activeVideoMinFrameDuration = CMTimeMake(value: 1, timescale: 30)
            
            if device.isFocusModeSupported(.continuousAutoFocus) {
                device.focusMode = .continuousAutoFocus
                
                if device.isSmoothAutoFocusSupported {
                    device.isSmoothAutoFocusEnabled = true
                }
            }
            
            if device.isExposureModeSupported(.continuousAutoExposure) {
                device.exposureMode = .continuousAutoExposure
            }
            
            if device.isWhiteBalanceModeSupported(.continuousAutoWhiteBalance) {
                device.whiteBalanceMode = .continuousAutoWhiteBalance
            }
            
            if device.isLowLightBoostSupported {
                device.automaticallyEnablesLowLightBoostWhenAvailable = true
            }
            
            device.unlockForConfiguration()
            
        } catch {
            print("Error locking device configuration")
        }
        
    }
    
}

// MARK: - CameraHandlerDelegate
extension CameraService: CameraHandlerDelegate {
    
    func cameraHandler(metaRect forCameraHandler: CameraHandler) -> CGRect {
        let rect = delegate?.cameraService(targetRect: self) ?? .zero
        
        let metaRect = CGRect(x: rect.origin.y / UIScreen.main.bounds.height,
                              y: rect.origin.x / UIScreen.main.bounds.width,
                              width: rect.height / UIScreen.main.bounds.height,
                              height: rect.width / UIScreen.main.bounds.width)
        return metaRect
    }
    
    func cameraHandler(devicePosition forCameraHandler: CameraHandler) -> AVCaptureDevice.Position {
        return position
    }
    
    func cameraHandler(_ cameraHandler: CameraHandler, didTake asset: AVAsset, url: URL) {
        DispatchQueue.main.async { [weak self] in
            guard let self = self else {
                return
            }
            self.delegate?.cameraService(self, didTake: asset, url: url)
        }
    }
    
}
