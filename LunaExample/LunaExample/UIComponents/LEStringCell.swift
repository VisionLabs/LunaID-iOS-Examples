//
//  LEStringCell.swift
//  LunaExample
//
//  Created by Kirill Zhadaev on 03.09.2024.
//

import UIKit

class LEStringCell: UITableViewCell {

    private let SideOffset: CGFloat = 10
    private let VerticalOffset: CGFloat = 20

    private let nameLabel: UILabel = {
        let label = UILabel(frame: .zero)
        label.translatesAutoresizingMaskIntoConstraints = false
        label.backgroundColor = .clear
        label.font = Fonts.etelka_17
        label.textColor = UIColor(named: "LunaBlue")
        return label
    }()

    private let valueLabel: UILabel = {
        let label = UILabel(frame: .zero)
        label.translatesAutoresizingMaskIntoConstraints = false
        label.backgroundColor = .clear
        label.font = Fonts.etelka_17
        label.textColor = UIColor.systemGreen
        label.textAlignment = .right
        return label
    }()

    override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
        super.init(style: style, reuseIdentifier: reuseIdentifier)

        createLayout()
    }

    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    //  MARK: - Configure -

    func configureCell(_ name: String, _ value: String?) {
        nameLabel.text = name
        valueLabel.text = value ?? "settings.empty_value".localized()
        valueLabel.textColor = value == nil ? UIColor.lightGray : UIColor.systemGreen
    }

    //  MARK: - Routine -

    private func createLayout() {
        contentView.backgroundColor = .clear
        backgroundColor = .clear
        selectionStyle = .none
        accessoryType = .disclosureIndicator

        contentView.addSubview(nameLabel)
        contentView.addSubview(valueLabel)

        NSLayoutConstraint.activate([
            nameLabel.topAnchor.constraint(equalTo: contentView.topAnchor, constant: SideOffset),
            nameLabel.leadingAnchor.constraint(equalTo: contentView.leadingAnchor, constant: SideOffset),
            nameLabel.trailingAnchor.constraint(greaterThanOrEqualTo: valueLabel.leadingAnchor, constant: -SideOffset),
            nameLabel.bottomAnchor.constraint(equalTo: contentView.bottomAnchor, constant: -SideOffset),

            valueLabel.topAnchor.constraint(equalTo: contentView.topAnchor, constant: VerticalOffset),
            valueLabel.leadingAnchor.constraint(equalTo: nameLabel.trailingAnchor),
            valueLabel.trailingAnchor.constraint(equalTo: contentView.trailingAnchor, constant: -SideOffset),
            valueLabel.bottomAnchor.constraint(equalTo: contentView.bottomAnchor, constant: -VerticalOffset),
        ])

        nameLabel.setContentHuggingPriority(.defaultHigh, for: .horizontal)
        valueLabel.setContentCompressionResistancePriority(.defaultLow, for: .horizontal)
    }
}

