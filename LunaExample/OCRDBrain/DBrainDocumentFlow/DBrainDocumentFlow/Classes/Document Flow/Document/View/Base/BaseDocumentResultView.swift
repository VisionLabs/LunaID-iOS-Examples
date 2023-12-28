//
//  BaseDocumentResultView.swift
//  dbraion
//
//  Created by Александрк Бельковский on 08/06/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import UIKit

class BaseDocumentResultView: UIView {
    
    var scrollView: UIScrollView = {
        let scrollView = UIScrollView()
        scrollView.translatesAutoresizingMaskIntoConstraints = false
        scrollView.backgroundColor = .white
        scrollView.bounces = true
        return scrollView
    }()
    
    var stackView: UIStackView = {
        let view = UIStackView()
        view.axis = .vertical
        view.spacing = 36.0
        view.backgroundColor = .white
        view.translatesAutoresizingMaskIntoConstraints = false
        
        return view
    }()

    var retakeButton: LoadingButton = {
        let button = LoadingButton(type: .system)
        button.translatesAutoresizingMaskIntoConstraints = false
        button.backgroundColor = UIColor(red: 17.0 / 255.0, green: 17.0 / 255.0, blue: 17.0 / 255.0, alpha: 1.0)
        let title = "Continue"
        let attributedString = NSMutableAttributedString(string: title)
        let range = NSRange(location: 0, length: attributedString.length)
        let textColor = UIColor(red: 250.0 / 255.0, green: 250.0 / 255.0, blue: 250.0 / 255.0, alpha: 1.0)
        attributedString.addAttribute(.kern, value: -0.32, range: range)
        attributedString.addAttribute(.font, value: UIFont.systemFont(ofSize: 16.0, weight: .semibold), range: range)
        attributedString.addAttribute(.foregroundColor, value: textColor, range: range)
        button.setAttributedTitle(attributedString, for: .normal)
        button.layer.cornerRadius = 24.0
        button.layer.masksToBounds = true
        
        return button
    }()
    
    // MARK: - Lifecycle
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        
        if #available(iOS 11.0, *) {
            scrollView.contentInset = UIEdgeInsets(top: 30.0 + safeAreaInsets.top, left: 0.0, bottom: 60.0 + safeAreaInsets.bottom, right: 0.0)
        } else {
            scrollView.contentInset = UIEdgeInsets(top: 30.0, left: 0.0, bottom: 60.0, right: 0.0)
        }
        
        addSubview(scrollView)
        scrollView.addSubview(stackView)
        scrollView.addSubview(retakeButton)
        
        let header = DocumentHeaderView()
        stackView.addArrangedSubview(header)
        
        setupConstraints()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    // MARK: - Layout
    
    private func setupConstraints() {
        scrollView.leadingAnchor.constraint(equalTo: leadingAnchor).isActive = true
        scrollView.topAnchor.constraint(equalTo: topAnchor).isActive = true
        scrollView.trailingAnchor.constraint(equalTo: trailingAnchor).isActive = true
        scrollView.bottomAnchor.constraint(equalTo: bottomAnchor).isActive = true
        
        stackView.widthAnchor.constraint(equalTo: widthAnchor).isActive = true
        stackView.leadingAnchor.constraint(equalTo: scrollView.leadingAnchor).isActive = true
        stackView.topAnchor.constraint(equalTo: scrollView.topAnchor).isActive = true
        stackView.trailingAnchor.constraint(equalTo: scrollView.trailingAnchor).isActive = true
        stackView.bottomAnchor.constraint(equalTo: retakeButton.topAnchor, constant: -60.0).isActive = true
        
        retakeButton.heightAnchor.constraint(equalToConstant: 48.0).isActive = true
        retakeButton.widthAnchor.constraint(equalToConstant: 141.0).isActive = true
        retakeButton.centerXAnchor.constraint(equalTo: scrollView.centerXAnchor).isActive = true
        retakeButton.bottomAnchor.constraint(equalTo: scrollView.bottomAnchor).isActive = true
    }
    
}
