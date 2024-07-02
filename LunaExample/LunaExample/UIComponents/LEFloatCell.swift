//
//  LEFloatCell.swift
//  LunaExample
//
//  Created by cipher on 14.02.2023.
//
import UIKit
import CoreGraphics

class LEFloatCell: UITableViewCell {

    private let SideOffset: CGFloat = 10
    private let VerticalOffset: CGFloat = 20

    private let valueTitleLabel = UILabel(frame: .zero)
    private let valueLabel = UILabel(frame: .zero)

    override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
        super.init(style: style, reuseIdentifier: reuseIdentifier)
        
        createLayout()
    }

    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    public func configureCell(_ title: String, _ value: CGFloat) {
        valueTitleLabel.text = title
        var dim = Double(value)
        var digitsCount = 0
        while(dim.truncatingRemainder(dividingBy: 1) > 0) {
            dim = dim * 10
            digitsCount += 1
        }
        
        if digitsCount - 1 > 1 {
            valueLabel.text = String(format: "%.2f", value)
        }
        else {
            valueLabel.text = String(format: "%.1f", value)
        }

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
        valueTitleLabel.textColor = UIColor(named: "LunaBlue")
        contentView.addSubview(valueTitleLabel)
        
        valueLabel.translatesAutoresizingMaskIntoConstraints = false
        valueLabel.backgroundColor = .clear
        valueLabel.font = Fonts.etelka_17
        valueLabel.textColor = UIColor.systemGreen
        valueLabel.textAlignment = .right
        contentView.addSubview(valueLabel)
        
        NSLayoutConstraint.activate([
            valueTitleLabel.topAnchor.constraint(equalTo: contentView.topAnchor, constant: VerticalOffset),
            valueTitleLabel.leadingAnchor.constraint(equalTo: contentView.leadingAnchor, constant: SideOffset),
            valueTitleLabel.trailingAnchor.constraint(greaterThanOrEqualTo: contentView.centerXAnchor),
            valueTitleLabel.bottomAnchor.constraint(equalTo: contentView.bottomAnchor, constant: -VerticalOffset),

            valueLabel.topAnchor.constraint(equalTo: contentView.topAnchor, constant: VerticalOffset),
            valueLabel.leadingAnchor.constraint(lessThanOrEqualTo: contentView.centerXAnchor),
            valueLabel.trailingAnchor.constraint(equalTo: contentView.trailingAnchor, constant: -SideOffset),
            valueLabel.bottomAnchor.constraint(equalTo: contentView.bottomAnchor, constant: -VerticalOffset),
        ])
    }
    
}

