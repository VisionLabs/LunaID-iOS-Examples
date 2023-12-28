//
//  DocumentHeaderView.swift
//  Test
//
//  Created by Александрк Бельковский on 04.03.2020.
//  Copyright © 2020 Александрк Бельковский. All rights reserved.
//

import UIKit

class DocumentHeaderView: UIView {
    
    let titleLabel: UILabel = {
        let label = UILabel()
        label.translatesAutoresizingMaskIntoConstraints = false
        label.font = UIFont.systemFont(ofSize: 12.0, weight: .semibold)
        label.textColor = UIColor(red: 17.0 / 255.0, green: 17.0 / 255.0, blue: 17.0 / 255.0, alpha: 1.0)
        label.text = "Field"
        
        return label
    }()
    
    let confidenceLabel: UILabel = {
        let label = UILabel()
        label.translatesAutoresizingMaskIntoConstraints = false
        label.font = UIFont.systemFont(ofSize: 12.0, weight: .semibold)
        label.textColor = UIColor(red: 17.0 / 255.0, green: 17.0 / 255.0, blue: 17.0 / 255.0, alpha: 1.0)
        label.text = "Confidence"
        
        return label
    }()
    
    let valueLabel: UILabel = {
        let label = UILabel()
        label.translatesAutoresizingMaskIntoConstraints = false
        label.font = UIFont.systemFont(ofSize: 12.0, weight: .semibold)
        label.textColor = UIColor(red: 17.0 / 255.0, green: 17.0 / 255.0, blue: 17.0 / 255.0, alpha: 1.0)
        label.text = "Value"
        
        return label
    }()
    
    // MARK: - Lifecycle
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        
        translatesAutoresizingMaskIntoConstraints = false
        
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
        heightAnchor.constraint(equalToConstant: 30.0).isActive = true
        
        titleLabel.widthAnchor.constraint(equalTo: widthAnchor, multiplier: 0.24).isActive = true
        titleLabel.leadingAnchor.constraint(equalTo: leadingAnchor, constant: 17.0).isActive = true
        titleLabel.topAnchor.constraint(equalTo: topAnchor).isActive = true
        
        confidenceLabel.leadingAnchor.constraint(equalTo: titleLabel.trailingAnchor).isActive = true
        confidenceLabel.topAnchor.constraint(equalTo: topAnchor).isActive = true
        
        valueLabel.widthAnchor.constraint(equalTo: widthAnchor, multiplier: 0.43).isActive = true
        valueLabel.topAnchor.constraint(equalTo: topAnchor).isActive = true
        valueLabel.trailingAnchor.constraint(equalTo: trailingAnchor, constant: -17.0).isActive = true
    }
    
}

