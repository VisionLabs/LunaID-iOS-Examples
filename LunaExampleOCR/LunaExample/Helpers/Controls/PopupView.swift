//
//  PopupView.swift
//  LunaExample
//
//  Created by IVAN CHIRKOV on 19.11.2020.
//

import UIKit

class PopupView: UIView {
    let cornerRadius: CGFloat = 16
    let shadowColor: UIColor = .black
    let shadowRadius: CGFloat = 12
    let shadowOpacity: Float = 0.2
    let shadowOffset: CGSize = CGSize(width: 0, height: 4)
    
    override func awakeFromNib() {
        super.awakeFromNib()
        
        layer.cornerRadius = cornerRadius
        layer.shadowColor = shadowColor.cgColor
        layer.shadowRadius = shadowRadius
        layer.shadowOpacity = shadowOpacity
        layer.shadowOffset = shadowOffset
        
    }
    
    @IBAction private func close() {
        removeFromSuperview()
    }

}
