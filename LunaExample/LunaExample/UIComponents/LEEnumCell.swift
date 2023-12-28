//
//  LEEnumCell.swift
//  LunaExample
//
//  Created by cipher on 14.02.2023.
//
import UIKit
import CoreGraphics

class LEEnumCell: UITableViewCell {

    private let SideOffset: CGFloat = 10
    
    private let valueTitleLabel = UILabel(frame: .zero)
    private let valueLabel = UILabel(frame: .zero)

    override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
        super.init(style: style, reuseIdentifier: reuseIdentifier)
        
        createLayout()
    }

    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    public func configureCell(_ title: String, _ value: String) {
        valueTitleLabel.text = title
        valueLabel.text = value

        contentView.backgroundColor = .clear
        backgroundColor = .clear
        selectionStyle = .none
        accessoryType = .disclosureIndicator
    }

    private func createLayout() {
        contentView.backgroundColor = .clear
        backgroundColor = .clear
        selectionStyle = .none
        accessoryType = .disclosureIndicator

        valueTitleLabel.translatesAutoresizingMaskIntoConstraints = false
        valueTitleLabel.backgroundColor = .clear
        valueTitleLabel.font = Fonts.etelka_17
        valueTitleLabel.textColor = .lunaBlue()
        contentView.addSubview(valueTitleLabel)
        
        valueLabel.translatesAutoresizingMaskIntoConstraints = false
        valueLabel.backgroundColor = .clear
        valueLabel.font = Fonts.etelka_17
        valueLabel.textColor = UIColor.systemGreen
        valueLabel.textAlignment = .right
        contentView.addSubview(valueLabel)
        
        NSLayoutConstraint.activate([
            valueTitleLabel.topAnchor.constraint(equalTo: contentView.topAnchor, constant: SideOffset),
            valueTitleLabel.leadingAnchor.constraint(equalTo: contentView.leadingAnchor, constant: SideOffset),
            valueTitleLabel.trailingAnchor.constraint(greaterThanOrEqualTo: contentView.centerXAnchor),
            valueTitleLabel.bottomAnchor.constraint(equalTo: contentView.bottomAnchor, constant: -SideOffset),
            
            valueLabel.topAnchor.constraint(equalTo: contentView.topAnchor, constant: SideOffset),
            valueLabel.leadingAnchor.constraint(lessThanOrEqualTo: contentView.centerXAnchor),
            valueLabel.trailingAnchor.constraint(equalTo: contentView.trailingAnchor, constant: -SideOffset),
            valueLabel.bottomAnchor.constraint(equalTo: contentView.bottomAnchor, constant: -SideOffset),
        ])
    }
    
}

