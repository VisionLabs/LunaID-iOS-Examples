//
//  LEOCRInstructionsView.swift
//  LunaMobile
//
//  Created by cipher on 03.03.2023.
//

import UIKit
import LunaCamera

class LEOCRInstructionsView: UIView {
    
    private let TextFontSize: CGFloat = 16
    private let TitleFontSize: CGFloat = 20
    private let TitlesBetween: CGFloat = 2
    private let InstructionsTopOffset: CGFloat = 24
    private let BottomButtonsHeight: CGFloat = 44
    private let BottomButtonsBetween: CGFloat = 16
    
    public var captureIDButtonHandler: VoidHandler?
    public var cancelButtonHandler: VoidHandler?

    override init(frame: CGRect) {
        super.init(frame: .zero)
        
        createLayout()
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    private func createLayout() {
        let instructionsTitleLabel = createLabel()
        instructionsTitleLabel.font = .etelka(TitleFontSize)
        instructionsTitleLabel.text = "capture_id.title".localized()
        addSubview(instructionsTitleLabel)
        
        let instructionsSubtitleLabel = createLabel()
        instructionsSubtitleLabel.font = .etelka(TextFontSize)
        instructionsSubtitleLabel.text = "capture_id.subtitle".localized()
        addSubview(instructionsSubtitleLabel)
        
        let instructionsLabel = createLabel()
        instructionsLabel.font = .etelka(TextFontSize)
        instructionsLabel.text = "capture_id.rules".localized()
        addSubview(instructionsLabel)
        
        let captureIDButton = LCRoundButton(type: .custom)
        captureIDButton.translatesAutoresizingMaskIntoConstraints = false
        captureIDButton.setTitle("capture_id.button".localized(), for: .normal)
        captureIDButton.addTarget(self, action: #selector(captureIDButtonTapped), for: .touchUpInside)
        addSubview(captureIDButton)
        
        let cancelButton = LCRoundButton(type: .custom)
        cancelButton.translatesAutoresizingMaskIntoConstraints = false
        cancelButton.setTitle("cancel.button".localized(), for: .normal)
        cancelButton.addTarget(self, action: #selector(cancelButtonTapped), for: .touchUpInside)
        addSubview(cancelButton)

        NSLayoutConstraint.activate([
            instructionsTitleLabel.topAnchor.constraint(equalTo: topAnchor),
            instructionsTitleLabel.leadingAnchor.constraint(equalTo: leadingAnchor),
            instructionsTitleLabel.trailingAnchor.constraint(equalTo: trailingAnchor),
            
            instructionsSubtitleLabel.topAnchor.constraint(equalTo: instructionsTitleLabel.bottomAnchor, constant: TitlesBetween),
            instructionsSubtitleLabel.leadingAnchor.constraint(equalTo: leadingAnchor),
            instructionsSubtitleLabel.trailingAnchor.constraint(equalTo: trailingAnchor),
            
            instructionsLabel.topAnchor.constraint(equalTo: instructionsSubtitleLabel.bottomAnchor, constant: InstructionsTopOffset),
            instructionsLabel.leadingAnchor.constraint(equalTo: leadingAnchor),
            instructionsLabel.trailingAnchor.constraint(equalTo: trailingAnchor),
            
            captureIDButton.bottomAnchor.constraint(equalTo: cancelButton.topAnchor, constant: -BottomButtonsBetween),
            captureIDButton.heightAnchor.constraint(equalToConstant: BottomButtonsHeight),
            captureIDButton.leadingAnchor.constraint(equalTo: leadingAnchor),
            captureIDButton.trailingAnchor.constraint(equalTo: trailingAnchor),

            cancelButton.heightAnchor.constraint(equalToConstant: BottomButtonsHeight),
            cancelButton.leadingAnchor.constraint(equalTo: leadingAnchor),
            cancelButton.trailingAnchor.constraint(equalTo: trailingAnchor),
            cancelButton.bottomAnchor.constraint(equalTo: safeAreaLayoutGuide.bottomAnchor, constant: -BottomButtonsBetween),
        ])
    }
    
    private func createLabel() -> UILabel {
        let newLabel = UILabel(frame: .zero)
        
        newLabel.translatesAutoresizingMaskIntoConstraints = false
        newLabel.numberOfLines = 0
        newLabel.backgroundColor = .clear
        
        return newLabel
    }

    @objc
    private func cancelButtonTapped(_ sender: UIButton) {
        cancelButtonHandler?()
    }

    @objc
    private func captureIDButtonTapped(_ sender: UIButton) {
        captureIDButtonHandler?()
    }
    
}
