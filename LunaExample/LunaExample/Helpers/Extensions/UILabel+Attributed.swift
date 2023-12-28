//
//  UILabel+Formating.swift
//  FaceEngineMobileDemo
//
//  Created on 07.11.16.
//  Copyright © 2016 Vision Labs. All rights reserved.
//

import UIKit

extension UILabel {
    
    // Трекинг (растояние между символами). Совпадает со значеним в Sketch.
    @IBInspectable var kern: CGFloat {
        get {
            return 0
        }
        set {
            if let attributedText = attributedText {
                let attr = NSMutableAttributedString(attributedString: attributedText)
                attr.addAttribute(.kern, value: newValue, range: NSRange(location: 0, length: attr.string.count))
                self.attributedText = attr
            }
        }
    }
    
}
