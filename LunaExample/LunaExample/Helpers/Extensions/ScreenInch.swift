//
//  ScreenInch.swift
//  Pods
//
//  Created on 15.08.16.
//
//

import UIKit

public enum DeviceInch {
    // iPhones
    case inch3_5
    case inch4
    case inch4_7
    case inch5_5
    case inch5_8
    case inch6_1 // Xr, 12, 12 pro
    case inch6_5 // Xs Max
    case inch6_7 // 12 pro max
    case unknown
    
    public init() {
        let screenSize = UIScreen.main.bounds.size
        let scale = UIScreen.main.scale
        switch (screenSize.width, screenSize.height, scale) {
        case (320, 480, 2):    self = .inch3_5
        case (320, 568, 2):    self = .inch4
        case (375, 667, 2):    self = .inch4_7
        case (414, 736, 3):    self = .inch5_5
        case (375, 812, 3):    self = .inch5_8
        case (414, 896, 2):    self = .inch6_1
        case (390, 844, 3):    self = .inch6_1
        case (414, 896, 3):    self = .inch6_5
        case (428, 926, 3):    self = .inch6_7
        default:               self = .unknown
        }
    }
    
    public static var current: DeviceInch {
        return DeviceInch()
    }
    
}

extension NSLayoutConstraint {
    
    @IBInspectable var inch3_5 : CGFloat {
        get { return 0 }
        set {
            setConstant(newValue, deviceType: .inch3_5)
        }
    }
    
    @IBInspectable var inch4 : CGFloat {
        get { return 0 }
        set {
            setConstant(newValue, deviceType: .inch4)
        }
    }
    
    @IBInspectable var inch4_7 : CGFloat {
        get { return 0 }
        set {
            setConstant(newValue, deviceType: .inch4_7)
        }
    }
    
    @IBInspectable var inch5_5 : CGFloat {
        get { return 0 }
        set {
            setConstant(newValue, deviceType: .inch5_5)
        }
    }
    
    @IBInspectable var inch5_8 : CGFloat {
        get { return 0 }
        set {
            setConstant(newValue, deviceType: .inch5_8)
        }
    }
    
    @IBInspectable var inch6_1 : CGFloat {
        get { return 0 }
        set {
            setConstant(newValue, deviceType: .inch6_1)
        }
    }
    
    @IBInspectable var inch6_5 : CGFloat {
        get { return 0 }
        set {
            setConstant(newValue, deviceType: .inch6_5)
        }
    }
    
    @IBInspectable var inch6_7 : CGFloat {
        get { return 0 }
        set {
            setConstant(newValue, deviceType: .inch6_7)
        }
    }
    
    
    private func setConstant(_ value : CGFloat, deviceType : DeviceInch) {
        let currentDeviceType = DeviceInch()
        guard currentDeviceType != .unknown else {
            print("Could not determine the device!")
            return
        }
        if currentDeviceType == deviceType {
            self.constant = value
        }
    }
    
}
