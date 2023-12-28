//
//  SettingsVersionCell.swift
//  LunaExample
//
//  Created by cipher on 14.02.2023.
//
import UIKit

class SettingsVersionCell: UITableViewCell {

    private let SideOffset: CGFloat = 10
    
    private let versionsLabel = UILabel(frame: .zero)

    override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
        super.init(style: style, reuseIdentifier: reuseIdentifier)
        
        createLayout()
    }

    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    public func configureCell() {
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

        versionsLabel.translatesAutoresizingMaskIntoConstraints = false
        versionsLabel.backgroundColor = .clear
        versionsLabel.font = Fonts.etelka_17
        versionsLabel.textColor = UIColor(named: "LunaBlue")
        versionsLabel.text = "VERSIONS"
        contentView.addSubview(versionsLabel)
        
        NSLayoutConstraint.activate([
            versionsLabel.topAnchor.constraint(equalTo: contentView.topAnchor, constant: SideOffset),
            versionsLabel.leadingAnchor.constraint(equalTo: contentView.leadingAnchor, constant: SideOffset),
            versionsLabel.trailingAnchor.constraint(equalTo: contentView.trailingAnchor, constant: -SideOffset),
            versionsLabel.bottomAnchor.constraint(equalTo: contentView.bottomAnchor, constant: -SideOffset),
        ])
    }
    
}

