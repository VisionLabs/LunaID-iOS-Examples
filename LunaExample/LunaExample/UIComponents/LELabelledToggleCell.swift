//
//  LELabelledToggleCell.swift
//  LunaExample
//
//  Created by cipher on 14.02.2023.
//

import UIKit

typealias ToggleStatusBlock = (Bool) -> Void

class LELabelledToggleCell: UITableViewCell {
    
    public var toggleStatusHandler: ToggleStatusBlock?
    
    private let SideOffset: CGFloat = 10
    private let VerticalOffset: CGFloat = 20

    private let statusLabel = UILabel(frame: .zero)
    private let switcher = UISwitch(frame: .zero)
    
    override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
        super.init(style: style, reuseIdentifier: reuseIdentifier)
        
        createLayout()
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    public func configureCell(_ initialState: Bool, _ title: String) {
        switcher.setOn(initialState, animated: true)
        statusLabel.text = title
    }
    
    private func createLayout() {
        contentView.backgroundColor = .clear
        backgroundColor = .clear
        selectionStyle = .none
        
        switcher.translatesAutoresizingMaskIntoConstraints = false
        switcher.backgroundColor = .clear
        switcher.addTarget(self, action: #selector(switcherStateChanged), for: .valueChanged)
        switcher.tintColor = UIColor(named: "LunaBlue")
        switcher.onTintColor = UIColor(named: "LunaBlue")
        contentView.addSubview(switcher)
        
        statusLabel.translatesAutoresizingMaskIntoConstraints = false
        statusLabel.backgroundColor = .clear
        statusLabel.font = Fonts.etelka_17
        statusLabel.textColor = UIColor(named: "LunaBlue")
        statusLabel.numberOfLines = 0
        contentView.addSubview(statusLabel)

        NSLayoutConstraint.activate([
            statusLabel.topAnchor.constraint(equalTo: contentView.topAnchor, constant: VerticalOffset),
            statusLabel.leadingAnchor.constraint(equalTo: contentView.leadingAnchor, constant: SideOffset),
            statusLabel.bottomAnchor.constraint(equalTo: contentView.bottomAnchor, constant: -VerticalOffset),
            statusLabel.trailingAnchor.constraint(lessThanOrEqualTo: switcher.leadingAnchor, constant: -SideOffset),

            switcher.trailingAnchor.constraint(equalTo: contentView.trailingAnchor, constant: -SideOffset),
            switcher.leadingAnchor.constraint(equalTo: statusLabel.trailingAnchor, constant: SideOffset),
            switcher.centerYAnchor.constraint(equalTo: contentView.centerYAnchor),
        ])

        switcher.setContentHuggingPriority(.defaultHigh, for: .horizontal)
        switcher.setContentCompressionResistancePriority(.required, for: .horizontal)
    }
    
    @objc
    private func switcherStateChanged(_ sender: UISwitch) {
        toggleStatusHandler?(sender.isOn)
    }
    
}

