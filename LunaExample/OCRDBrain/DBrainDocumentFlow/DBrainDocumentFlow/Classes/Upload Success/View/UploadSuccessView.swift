//
//  UploadSuccessView.swift
//  Test
//
//  Created by Александрк Бельковский on 20/03/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import UIKit

class UploadSuccessView: UIView {
    
    private var labelContainterView: UIView = {
        let view = UIView()
        view.translatesAutoresizingMaskIntoConstraints = false
        
        return view
    }()
    
    var successLabel: UILabel = {
        let label = UILabel()
        label.textAlignment = .center
        label.numberOfLines = 0
        label.translatesAutoresizingMaskIntoConstraints = false
        
        let title = "Thanks! We’ll review your\ndocuments within 24 hours"
        let attributedString = NSMutableAttributedString(string: title)
        let range = NSRange(location: 0, length: attributedString.length)
        let textColor = UIColor(red: 25.0 / 255.0, green: 28.0 / 255.0, blue: 31.0 / 255.0, alpha: 1.0)
        attributedString.addAttribute(.kern, value: 0.38, range: range)
        attributedString.addAttribute(.font, value: UIFont.systemFont(ofSize: 20.0, weight: .medium), range: range)
        attributedString.addAttribute(.foregroundColor, value: textColor, range: range)
        label.attributedText = attributedString
        
        return label
    }()
    
    var doneButton: UIButton = {
        let button = UIButton(type: .system)
        button.translatesAutoresizingMaskIntoConstraints = false
        
        button.backgroundColor = UIColor(red: 235.0 / 255.0, green: 0.0 / 255.0, blue: 141.0 / 255.0, alpha: 1.0)
        button.setTitleColor(.white, for: .normal)
        
        let title = "Got it"
        let attributedString = NSMutableAttributedString(string: title)
        let range = NSRange(location: 0, length: attributedString.length)
        attributedString.addAttribute(.kern, value: -0.32, range: range)
        attributedString.addAttribute(.font, value: UIFont.systemFont(ofSize: 16.0, weight: .semibold), range: range)
        attributedString.addAttribute(.foregroundColor, value: UIColor.white, range: range)
        button.setAttributedTitle(attributedString, for: .normal)
        
        button.layer.cornerRadius = 24.0
        button.layer.masksToBounds = true
        
        return button
    }()
    
    // MARK: - Lifecycle
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        
        backgroundColor = UIColor.white
        
        addSubview(doneButton)
        addSubview(labelContainterView)
        labelContainterView.addSubview(successLabel)
        
        setupConstraints()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    // MARK: - Layout
    
    private func setupConstraints() {
        labelContainterView.leadingAnchor.constraint(equalTo: leadingAnchor).isActive = true
        labelContainterView.topAnchor.constraint(equalTo: topAnchor).isActive = true
        labelContainterView.trailingAnchor.constraint(equalTo: trailingAnchor).isActive = true
        labelContainterView.bottomAnchor.constraint(equalTo: doneButton.topAnchor).isActive = true
        
        successLabel.leadingAnchor.constraint(equalTo: leadingAnchor, constant: 32.0).isActive = true
        successLabel.trailingAnchor.constraint(equalTo: trailingAnchor, constant: -32.0).isActive = true
        successLabel.centerYAnchor.constraint(equalTo: labelContainterView.centerYAnchor).isActive = true
        
        doneButton.heightAnchor.constraint(equalToConstant: 48.0).isActive = true
        doneButton.leadingAnchor.constraint(equalTo: leadingAnchor, constant: 32.0).isActive = true
        doneButton.trailingAnchor.constraint(equalTo: trailingAnchor, constant: -32.0).isActive = true
        
        if #available(iOS 11.0, *) {
            doneButton.bottomAnchor.constraint(equalTo: safeAreaLayoutGuide.bottomAnchor, constant: -24.0).isActive = true
        } else {
            doneButton.bottomAnchor.constraint(equalTo: bottomAnchor, constant: -24.0).isActive = true
        }
    }
    
}
