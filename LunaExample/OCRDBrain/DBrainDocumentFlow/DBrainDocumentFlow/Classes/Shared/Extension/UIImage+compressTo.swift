//
//  UIImage+compressTo.swift
//  Test
//
//  Created by Александрк Бельковский on 19.04.2020.
//  Copyright © 2020 Александрк Бельковский. All rights reserved.
//
import UIKit

extension UIImage {
    
    func compressTo(expectedSizeKb kb: Int) -> Data? {
        let sizeInBytes = kb * 1000//* 1024 * 1024
        var needCompress: Bool = true
        var imgData: Data? = self.jpegData(compressionQuality: 1.0)
        var compressingValue: CGFloat = 1.0
        while (needCompress && compressingValue > 0.0) {
            if let data: Data = self.jpegData(compressionQuality: compressingValue) {
                if data.count < sizeInBytes {
                    needCompress = false
                    imgData = data
                } else {
                    compressingValue -= 0.05
                }
            }
        }

        return imgData
    }
    
}
