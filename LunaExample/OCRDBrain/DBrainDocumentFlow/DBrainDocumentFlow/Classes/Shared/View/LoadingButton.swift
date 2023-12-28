//
//  LoadingButton.swift
//  Test
//
//  Created by Александрк Бельковский on 17/04/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import UIKit

class LoadingButton: UIButton {
    
    var activityIndicator: UIActivityIndicatorView = {
        let activityIndicator = UIActivityIndicatorView()
        activityIndicator.style = .white
        activityIndicator.hidesWhenStopped = true
        activityIndicator.translatesAutoresizingMaskIntoConstraints = false
        
        return activityIndicator
    }()
    
    var tempTitleColor: UIColor?
    
    var tempAttributedTitle: NSAttributedString?
    
    var isLoading: Bool = false {
        didSet {
            if isLoading {
                guard !activityIndicator.isAnimating else {
                    return
                }
                tempTitleColor = titleColor(for: .normal)?.withAlphaComponent(1.0)
                activityIndicator.color = titleColor(for: .normal)?.withAlphaComponent(1.0)
                activityIndicator.startAnimating()
                tintColor = .clear
                isEnabled = false
                setTitleColor(.clear, for: .normal)
                
                if let image = self.image(for: .normal) {
                    setImage(image.withRenderingMode(.alwaysTemplate), for: .normal)
                }
                
                tempAttributedTitle = attributedTitle(for: .normal)
                setAttributedTitle(nil, for: .normal)
            } else {
                if let tempTitleColor = tempTitleColor {
                    setTitleColor(tempTitleColor, for: .normal)
                }
                
                activityIndicator.stopAnimating()
                tintColor = self.activityIndicator.color
                isEnabled = true
                
                if let image = self.image(for: .normal) {
                    setImage(image.withRenderingMode(.alwaysOriginal), for: .normal)
                }
                
                if let tempAttributedTitle = tempAttributedTitle {
                    setAttributedTitle(tempAttributedTitle, for: .normal)
                    self.tempAttributedTitle = nil
                }
            }
        }
    }
    
    // MARK: - Lifecycle
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        
        addSubview(activityIndicator)
        
        setupConstraints()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    // MARK: - Layout
    
    private func setupConstraints() {
        activityIndicator.centerXAnchor.constraint(equalTo: centerXAnchor).isActive = true
        activityIndicator.centerYAnchor.constraint(equalTo: centerYAnchor).isActive = true
    }
    
}
