//
//  LEOCRFieldCell.swift
//  LunaAuth
//
//  Created by IVAN CHIRKOV on 27.04.2021.
//

import UIKit
import OCR
import LunaCamera

public class LEOCRFieldCell: UITableViewCell {

    private let LabelsTopOffset: CGFloat = 10
    private let LabelsBetween: CGFloat = 4
    private let TextFontSize: CGFloat = 16
    private let FieldTypeAlpha: CGFloat = 0.6

    private let fieldTypeLabel = UITextField(frame: .zero)
    private let valueLabel = UITextField(frame: .zero)
    
    public static let reuseID = "LEOCRFieldCell"

    override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
        super.init(style: style, reuseIdentifier: reuseIdentifier)
        
        createLayout()
        fieldTypeLabel.delegate = self
        valueLabel.delegate = self
    }

    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    private func createLayout() {
        fieldTypeLabel.font = .etelka(TextFontSize)
        fieldTypeLabel.textColor = .lunaGray().withAlphaComponent(FieldTypeAlpha)
        fieldTypeLabel.translatesAutoresizingMaskIntoConstraints = false
        fieldTypeLabel.backgroundColor = .clear
        contentView.addSubview(fieldTypeLabel)
        
        valueLabel.font = .etelka(TextFontSize)
        valueLabel.textColor = .lunaBlack()
        valueLabel.translatesAutoresizingMaskIntoConstraints = false
        valueLabel.backgroundColor = .clear
        contentView.addSubview(valueLabel)
        
        NSLayoutConstraint.activate([
            fieldTypeLabel.topAnchor.constraint(equalTo: contentView.topAnchor, constant: LabelsTopOffset),
            fieldTypeLabel.trailingAnchor.constraint(equalTo: contentView.trailingAnchor),
            fieldTypeLabel.leadingAnchor.constraint(equalTo: contentView.leadingAnchor),

            valueLabel.topAnchor.constraint(equalTo: fieldTypeLabel.bottomAnchor, constant: LabelsBetween),
            valueLabel.bottomAnchor.constraint(equalTo: contentView.bottomAnchor, constant: -LabelsTopOffset),
            valueLabel.trailingAnchor.constraint(equalTo: contentView.trailingAnchor),
            valueLabel.leadingAnchor.constraint(equalTo: contentView.leadingAnchor),
        ])
    }
    
    public func configureCell(_ typeName: String, _ fieldValue: String, isValueChangeable: Bool = false) {
        fieldTypeLabel.text = typeName
        fieldTypeLabel.isUserInteractionEnabled = false
        valueLabel.text = fieldValue
        valueLabel.isUserInteractionEnabled = isValueChangeable
    }

}

extension LEOCRFieldCell: UITextFieldDelegate {

    public func textFieldShouldReturn(_ textField: UITextField) -> Bool {
        if valueLabel.text == "" {
            return false
        }
        fieldTypeLabel.resignFirstResponder()
        valueLabel.resignFirstResponder()
        return true
    }
}
