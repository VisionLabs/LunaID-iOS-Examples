//
//  TabButton.swift
//  LunaExample
//
//  Created by IVAN CHIRKOV on 09.08.2021.
//

import UIKit
import LunaCamera

class TabButton: UIView {
    
    private let SpaceBetween: CGFloat = 10
    private let LabelFontSize: CGFloat = 17

    public var tabTitle = "" {
        didSet {
            tabTitleLabel.text = tabTitle
        }
    }
    
    public var tabIcon: UIImage? {
        didSet {
            tabIconView.image = tabIcon
        }
    }
    
    private let tabIconView = UIImageView(frame: .zero)
    private let tabTitleLabel = UILabel(frame: .zero)
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        
        createLayout()
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    private func createLayout() {
        clipsToBounds = true
        
        tabIconView.translatesAutoresizingMaskIntoConstraints = false
        tabIconView.backgroundColor = .clear
        addSubview(tabIconView)
            
        tabTitleLabel.translatesAutoresizingMaskIntoConstraints = false
        tabTitleLabel.backgroundColor = .clear
        tabTitleLabel.numberOfLines = 0
        tabTitleLabel.font = .etelka(LabelFontSize)
        tabTitleLabel.textColor = .lunaBlue()
        addSubview(tabTitleLabel)
        
        NSLayoutConstraint.activate([
            tabIconView.topAnchor.constraint(equalTo: topAnchor),
            tabIconView.centerXAnchor.constraint(equalTo: centerXAnchor),

            tabTitleLabel.topAnchor.constraint(equalTo: tabIconView.bottomAnchor, constant: SpaceBetween),
            tabTitleLabel.centerXAnchor.constraint(equalTo: centerXAnchor),
            tabTitleLabel.bottomAnchor.constraint(equalTo: bottomAnchor),
        ])
    }
    
}
