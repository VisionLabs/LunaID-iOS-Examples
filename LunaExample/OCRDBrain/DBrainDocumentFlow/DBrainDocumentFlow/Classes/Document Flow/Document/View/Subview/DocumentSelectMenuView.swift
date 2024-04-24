//
//  DocumentSelectMenuView.swift
//  dbraion
//
//  Created by Александрк Бельковский on 10/06/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import UIKit

class DocumentSelectMenuView: UIView {
    
    var dropDownButton: DocumentDropDownButtonView = {
        let button = DocumentDropDownButtonView(type: .system)
        button.contentHorizontalAlignment = .left
        button.contentVerticalAlignment = .center
        button.titleEdgeInsets.left = 24.0
        button.titleLabel?.font = UIFont.systemFont(ofSize: 16.0, weight: .medium)
        button.setTitleColor(UIColor(red: 250.0 / 255.0, green: 250.0 / 255.0, blue: 250.0 / 255.0, alpha: 1.0), for: .normal)
        button.backgroundColor = UIColor(red: 12.0 / 255.0, green: 198.0 / 255.0, blue: 109.0 / 255.0, alpha: 1.0)
        button.layer.cornerRadius = 24.0
        button.layer.masksToBounds = true
        button.translatesAutoresizingMaskIntoConstraints = false
        
        return button
    }()
    
    // MARK: - Lifecycle
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        
        backgroundColor = UIColor.white
        
        addSubview(dropDownButton)
        
        setupConstraints()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    // MARK: - Layout
    
    private func setupConstraints() {
        dropDownButton.heightAnchor.constraint(equalToConstant: 48.0).isActive = true
        dropDownButton.leftAnchor.constraint(equalTo: leftAnchor).isActive = true
        dropDownButton.topAnchor.constraint(equalTo: topAnchor).isActive = true
        dropDownButton.trailingAnchor.constraint(equalTo: trailingAnchor).isActive = true
        dropDownButton.bottomAnchor.constraint(equalTo: bottomAnchor).isActive = true
    }
    
}
