//
//  DocumentSelectView.swift
//  dbraion
//
//  Created by Александрк Бельковский on 10/06/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import UIKit

class DocumentSelectView: UIView {
    
    var backButton: UIButton = {
        let button = UIButton(type: .system)
        button.translatesAutoresizingMaskIntoConstraints = false
        button.titleLabel?.font = UIFont.systemFont(ofSize: 15.0, weight: .medium)
        button.setTitle("Go back", for: .normal)
        button.setTitleColor(UIColor(red: 19.0 / 255.0, green: 86.0 / 255.0, blue: 168.0 / 255.0, alpha: 1.0), for: .normal)
        
        return button
    }()
    
    let nationalitySelectMenuView: DocumentSelectMenuView = {
        let view = DocumentSelectMenuView()
        view.dropDownButton.setTitle("Russian", for: .normal)
        view.translatesAutoresizingMaskIntoConstraints = false
        
        return view
    }()
    
    var nextButton: LoadingButton = {
        let button = LoadingButton(type: .system)
        button.translatesAutoresizingMaskIntoConstraints = false
        
        button.backgroundColor = UIColor(red: 19.0 / 255.0, green: 86.0 / 255.0, blue: 168.0 / 255.0, alpha: 1.0)
        button.tintColor = UIColor.white
        
        let title = "Recognize"
        let attributedString = NSMutableAttributedString(string: title)
        let range = NSRange(location: 0, length: attributedString.length)
        attributedString.addAttribute(.kern, value: -0.32, range: range)
        attributedString.addAttribute(.font, value: UIFont.systemFont(ofSize: 23.0, weight: .semibold), range: range)
        attributedString.addAttribute(.foregroundColor, value: UIColor.white, range: range)
        button.setAttributedTitle(attributedString, for: .normal)
        
        button.layer.cornerRadius = 36.0
        button.layer.masksToBounds = true
        
        return button
    }()

    // MARK: - Lifecycle
    
    override init(frame: CGRect) {
        super.init(frame: frame)

        backgroundColor = .white
        
        addSubview(nationalitySelectMenuView)
        addSubview(nextButton)
        addSubview(backButton)
        
        setupConstraints()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    // MARK: - Layout
    
    private func setupConstraints() {
        nationalitySelectMenuView.centerYAnchor.constraint(equalTo: centerYAnchor).isActive = true
        nationalitySelectMenuView.leadingAnchor.constraint(equalTo: leadingAnchor, constant: 16.0).isActive = true
        nationalitySelectMenuView.trailingAnchor.constraint(equalTo: trailingAnchor, constant: -16.0).isActive = true
        
        nextButton.heightAnchor.constraint(equalToConstant: 72.0).isActive = true
        nextButton.widthAnchor.constraint(equalToConstant: 188.0).isActive = true
        nextButton.centerXAnchor.constraint(equalTo: centerXAnchor).isActive = true
        nextButton.bottomAnchor.constraint(equalTo: backButton.topAnchor, constant: -18.0).isActive = true
        
        backButton.centerXAnchor.constraint(equalTo: centerXAnchor).isActive = true
        backButton.heightAnchor.constraint(equalToConstant: 72.0).isActive = true
        backButton.bottomAnchor.constraint(equalTo: bottomAnchor, constant: -34.0).isActive = true
    }
    
}
