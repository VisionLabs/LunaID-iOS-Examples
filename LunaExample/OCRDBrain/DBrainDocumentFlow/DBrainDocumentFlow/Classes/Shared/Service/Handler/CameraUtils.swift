//
//  CameraUtils.swift
//  Test
//
//  Created by Александрк Бельковский on 17/04/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import UIKit
import Foundation
import AVFoundation
import Accelerate

class CameraUtils {
    
    private let context = CIContext()
    
    func image(from sampleBuffer: CMSampleBuffer) -> UIImage? {
        guard let imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer) else {
            return nil
        }
        
        let ciImage = CIImage(cvPixelBuffer: imageBuffer)
        
        guard let cgImage = context.createCGImage(ciImage, from: ciImage.extent) else {
            return nil
        }
        
        return UIImage(cgImage: cgImage)
    }
    
    func crop(image: UIImage, to outputRect: CGRect) -> UIImage? {
        let scale = image.scale
        let imageOrientation = image.imageOrientation
        
        guard let cgImage = image.cgImage else {
            return nil
        }
        
        let width = CGFloat(cgImage.width)
        let height = CGFloat(cgImage.height)
        
        let cropRect = CGRect(x: outputRect.origin.x * width,
                              y: outputRect.origin.y * height,
                              width: outputRect.size.width * width,
                              height: outputRect.size.height * height)
        
        guard let croppedCGImage = cgImage.cropping(to: cropRect) else {
            return nil
        }
        
        let result = UIImage(cgImage: croppedCGImage, scale: scale, orientation: imageOrientation)

        return result
    }
    
    func lumaHistogram(from imageRef: CGImage) -> [UInt]?
    {
        let inProvider = imageRef.dataProvider!
        let data = inProvider.data!
        
        let pointer = UnsafeMutableRawPointer(mutating: CFDataGetBytePtr(data))
        
        var inBuffer = vImage_Buffer(data: pointer,
                                     height: vImagePixelCount(imageRef.height),
                                     width: vImagePixelCount(imageRef.width),
                                     rowBytes: imageRef.bytesPerRow)
        
        let alpha = [vImagePixelCount](repeating: 0, count: 256)
        let red = [vImagePixelCount](repeating: 0, count: 256)
        let green = [vImagePixelCount](repeating: 0, count: 256)
        let blue = [vImagePixelCount](repeating: 0, count: 256)
        
        let alphaPtr = UnsafeMutablePointer<UInt>(mutating: alpha) as UnsafeMutablePointer<UInt>?
        let redPtr =  UnsafeMutablePointer<UInt>(mutating: red) as UnsafeMutablePointer<UInt>?
        let greenPtr = UnsafeMutablePointer<UInt>(mutating: green) as UnsafeMutablePointer<UInt>?
        let bluePtr = UnsafeMutablePointer<UInt>(mutating: blue) as UnsafeMutablePointer<UInt>?
        
        let rgba = [redPtr, greenPtr, bluePtr, alphaPtr]

        let histogram = UnsafeMutablePointer<UnsafeMutablePointer<UInt>?>(mutating: rgba)
        let error = vImageHistogramCalculation_ARGB8888(&inBuffer, histogram, UInt32(kvImageNoFlags))
        
        if error == kvImageNoError {
            var luma = [UInt]()
            
            for i in 0..<green.count {
                let r = Float(red[i]) * CameraConstans.redCoefficient
                let g = Float(green[i]) * CameraConstans.greenCoefficient
                let b = Float(blue[i]) * CameraConstans.blueCoefficient
                
                let l = UInt(r + g + b)
                
                luma.append(l)
            }
            
            return luma
        }
        
        return nil
    }
    
    func imageRotated(image: UIImage, by degrees: CGFloat) -> UIImage {
        let radians = degrees * CGFloat.pi / 180.0
        
        let rotatedViewBox = UIView(frame: CGRect(origin: .zero, size: image.size))
        let transform: CGAffineTransform = CGAffineTransform(rotationAngle: radians)
        rotatedViewBox.transform = transform
        let rotatedSize = rotatedViewBox.frame.size
        
        UIGraphicsBeginImageContextWithOptions(rotatedSize, false, image.scale)
        let bitmap = UIGraphicsGetCurrentContext()
        
        bitmap!.translateBy(x: rotatedSize.width / 2, y: rotatedSize.height / 2)
        bitmap!.rotate(by: radians)
        bitmap!.scaleBy(x: 1.0, y: -1.0)
        image.draw(in: CGRect(origin: CGPoint(x: -image.size.width / 2, y: -image.size.height / 2), size: image.size))
        let newImage = UIGraphicsGetImageFromCurrentImageContext()
        UIGraphicsEndImageContext()
        return newImage!
    }
    
    func scale(image: UIImage, to newSize: CGSize) -> UIImage {
        UIGraphicsBeginImageContext(newSize)
        
        let rect = CGRect(origin: .zero, size: newSize)
        
        image.draw(in: rect)
        let scaledImage = UIGraphicsGetImageFromCurrentImageContext()
        
        UIGraphicsEndImageContext()
        
        return scaledImage!
    }
    
    func normalized(image: UIImage) -> UIImage {
        guard image.imageOrientation != .up else { return image }
        
        UIGraphicsBeginImageContextWithOptions(image.size, false, image.scale)
        defer {
            UIGraphicsEndImageContext()
        }
        
        let rect = CGRect(x: 0, y: 0, width: image.size.width, height: image.size.height)
        image.draw(in: rect)
        
        if let normalizedImage = UIGraphicsGetImageFromCurrentImageContext() {
            return normalizedImage
        } else {
            return image
        }
    }
    
    func flippedHorizontally(image: UIImage) -> UIImage {
        var flippedOrientation = UIImage.Orientation.upMirrored
        
        switch image.imageOrientation {
        case .down:
            flippedOrientation = .down
        default:
            break
        }
        
        let image = UIImage(cgImage: image.cgImage!, scale: image.scale, orientation: flippedOrientation)
        return normalized(image: image)
    }
    
}
