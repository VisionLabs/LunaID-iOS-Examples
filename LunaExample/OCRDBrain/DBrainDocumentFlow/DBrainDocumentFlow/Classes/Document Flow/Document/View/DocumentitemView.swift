//
//  DocumentitemView.swift
//  Test
//
//  Created by Александрк Бельковский on 04.03.2020.
//  Copyright © 2020 Александрк Бельковский. All rights reserved.
//

import UIKit

private class InsetsLabel: UILabel {
    
    var insets: UIEdgeInsets = .zero {
        didSet {
            guard insets != oldValue else {
                return
            }
            
            sizeToFit()
        }
    }
    
    override func drawText(in rect: CGRect) {
        super.drawText(in: rect.inset(by: insets))
    }
    
    override var intrinsicContentSize: CGSize {
        var size = super.intrinsicContentSize
        size.width += insets.left + insets.right
        size.height += insets.top + insets.bottom
        
        return size
    }
    
}

class DocumentItemView: UIView {
    
    let titleLabel: UILabel = {
        let label = UILabel()
        label.translatesAutoresizingMaskIntoConstraints = false
        label.font = UIFont.systemFont(ofSize: 12.0, weight: .regular)
        label.textColor = UIColor(red: 17.0 / 255.0, green: 17.0 / 255.0, blue: 17.0 / 255.0, alpha: 1.0)
        label.numberOfLines = 0
        
        return label
    }()
    
    let confidenceLabel: UILabel = {
        let label = InsetsLabel()
        label.translatesAutoresizingMaskIntoConstraints = false
        label.insets = UIEdgeInsets(top: 0.0, left: 13.0, bottom: 0.0, right: 13.0)
        label.font = UIFont.systemFont(ofSize: 11.0, weight: .semibold)
        label.textColor = UIColor(red: 250.0 / 255.0, green: 250.0 / 255.0, blue: 250.0 / 255.0, alpha: 1.0)
        label.layer.cornerRadius = 12.0
        label.layer.masksToBounds = true
        
        return label
    }()
    
    let valueLabel: UILabel = {
        let label = UILabel()
        label.translatesAutoresizingMaskIntoConstraints = false
        label.font = UIFont.systemFont(ofSize: 12.0, weight: .regular)
        label.numberOfLines = 0
        label.textColor = UIColor(red: 17.0 / 255.0, green: 17.0 / 255.0, blue: 17.0 / 255.0, alpha: 1.0)
        
        return label
    }()
    
    // MARK: - Lifecycle
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        
        addSubview(titleLabel)
        addSubview(confidenceLabel)
        addSubview(valueLabel)
        
        setupConstraints()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    // MARK: - Layout
    
    private func setupConstraints() {
        titleLabel.widthAnchor.constraint(equalTo: widthAnchor, multiplier: 0.24).isActive = true
        titleLabel.leadingAnchor.constraint(equalTo: leadingAnchor, constant: 17.0).isActive = true
        titleLabel.topAnchor.constraint(equalTo: topAnchor, constant: 4.0).isActive = true
        titleLabel.bottomAnchor.constraint(lessThanOrEqualTo: bottomAnchor).isActive = true
        
        confidenceLabel.heightAnchor.constraint(equalToConstant: 24.0).isActive = true
        confidenceLabel.leadingAnchor.constraint(equalTo: titleLabel.trailingAnchor).isActive = true
        confidenceLabel.topAnchor.constraint(equalTo: topAnchor).isActive = true
        confidenceLabel.bottomAnchor.constraint(lessThanOrEqualTo: bottomAnchor).isActive = true
        
        valueLabel.widthAnchor.constraint(equalTo: widthAnchor, multiplier: 0.43).isActive = true
        valueLabel.topAnchor.constraint(equalTo: topAnchor, constant: 4.0).isActive = true
        valueLabel.trailingAnchor.constraint(equalTo: trailingAnchor, constant: -17.0).isActive = true
        valueLabel.bottomAnchor.constraint(lessThanOrEqualTo: bottomAnchor).isActive = true
    }
    
}
