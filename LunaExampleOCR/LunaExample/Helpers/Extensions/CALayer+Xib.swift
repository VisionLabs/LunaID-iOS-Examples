//
//  CALayer+Xib.swift
//  GlambookClient
//
//  Created on 04.02.16.
//  Copyright © 2016 Vision Labs. All rights reserved.
//

import UIKit

extension CALayer {
    
    var borderCGColor: UIColor? {
        get {
            guard let borderColor = self.borderColor else { return nil }
            return UIColor(cgColor: borderColor)
        }
        set {
            if let newValue = newValue {
                borderColor = newValue.cgColor
            }
        }
    }
    
    var shadowCGColor: UIColor? {
        get {
            guard let shadowColor = shadowColor else { return nil }
            return UIColor(cgColor: shadowColor)
        }
        set {
            if let newValue = newValue {
                shadowColor = newValue.cgColor
            }
        }
    }
    
}
