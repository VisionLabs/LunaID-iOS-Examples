//
//  TextField.swift
//  FaceEngineMobileDemo
//
//  Created on 01.11.16.
//  Copyright © 2016 Vision Labs. All rights reserved.
//

import UIKit

class TextField: UITextField {
    
    let lineWidth: CGFloat = 1
    let lineColor: UIColor = Colors.ghost.withAlphaComponent(0.3)
    let highlightedLineColor: UIColor = Colors.blue
    let invalidColor: UIColor = Colors.red
    let placeholderColor: UIColor = Colors.ghost
    
    private let line = CALayer()
    
    var isValid: Bool = true {
        didSet {
            if isValid {
                changeLineColor(isFirstResponder ? highlightedLineColor : lineColor)
            } else {
                changeLineColor(invalidColor)
            }
        }
    }
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        
        setupUI()
        
        addTarget(self, action: #selector(didBeginEditing), for: .editingDidBegin)
        addTarget(self, action: #selector(didEndEditing), for: .editingDidEnd)
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    override func layoutSubviews() {
        super.layoutSubviews()
        line.frame = CGRect(x: 0, y: frame.height - lineWidth, width: frame.width, height: lineWidth)
    }

    func setupUI() {
        borderStyle = .none
        
        if let placeholder = placeholder {
            attributedPlaceholder = NSAttributedString(
                string:
                placeholder, attributes: [
                    .foregroundColor: Colors.ghost,
                    .font: font!
                ]
            )
        }
        
        attributedText = NSAttributedString(
            string: "",
            attributes: [
                .foregroundColor: Colors.shark,
                .font: font!
            ]
        )
        
        // Line
        line.frame = CGRect(x: 0, y: frame.height - lineWidth, width: frame.width, height: lineWidth)
        line.backgroundColor = lineColor.cgColor
        layer.addSublayer(line)
    }
    
    @objc
    private func didBeginEditing() {
        changeLineColor(isValid ? highlightedLineColor : invalidColor)
    }
    
    @objc
    private func didEndEditing() {
        changeLineColor(isValid ? lineColor : invalidColor)
    }
    
    private func changeLineColor(_ color: UIColor) {
        line.backgroundColor = color.cgColor
    }

}
