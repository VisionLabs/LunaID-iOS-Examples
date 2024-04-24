//
//  LABestShotInstructionView.swift
//  LunaMobile
//
//  Created by cipher on 03.03.2023.
//

import UIKit
import LunaCamera

class LEBiometricInstructionsView: UIView {
    
    private let TextFontSize: CGFloat = 16
    private let TitleFontSize: CGFloat = 20
    private let TitlesBetween: CGFloat = 2
    private let InstructionsTopOffset: CGFloat = 24
    private let BottomButtonsHeight: CGFloat = 44
    private let BottomButtonsBetween: CGFloat = 16

    public var cancelButtonHandler: VoidHandler?
    public var captureBiometricButtonHandler: VoidHandler?

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
        instructionsTitleLabel.text = "capture_biometric.title".localized()
        addSubview(instructionsTitleLabel)
        
        let instructionsSubtitleLabel = createLabel()
        instructionsSubtitleLabel.font = .etelka(TextFontSize)
        instructionsSubtitleLabel.text = "capture_biometric.subtitle".localized()
        addSubview(instructionsSubtitleLabel)
        
        let instructionsLabel = createLabel()
        instructionsLabel.font = .etelka(TextFontSize)
        instructionsLabel.text = "capture_biometric.rules".localized()
        addSubview(instructionsLabel)
        
        let cancelButton = createButton()
        cancelButton.setTitle("cancel.button".localized(), for: .normal)
        cancelButton.addTarget(self, action: #selector(cancelButtonTapped), for: .touchUpInside)
        addSubview(cancelButton)
        
        let captureBiometricButton = createButton()
        captureBiometricButton.setTitle("capture_biometric.button".localized(), for: .normal)
        captureBiometricButton.addTarget(self, action: #selector(captureBiometricButtonTapped), for: .touchUpInside)
        addSubview(captureBiometricButton)
        
        NSLayoutConstraint.activate([
            instructionsTitleLabel.topAnchor.constraint(equalTo: safeAreaLayoutGuide.topAnchor),
            instructionsTitleLabel.leadingAnchor.constraint(equalTo: leadingAnchor),
            instructionsTitleLabel.trailingAnchor.constraint(equalTo: trailingAnchor),
            
            instructionsSubtitleLabel.topAnchor.constraint(equalTo: instructionsTitleLabel.bottomAnchor, constant: TitlesBetween),
            instructionsSubtitleLabel.leadingAnchor.constraint(equalTo: leadingAnchor),
            instructionsSubtitleLabel.trailingAnchor.constraint(equalTo: trailingAnchor),
            
            instructionsLabel.topAnchor.constraint(equalTo: instructionsSubtitleLabel.bottomAnchor, constant: InstructionsTopOffset),
            instructionsLabel.leadingAnchor.constraint(equalTo: leadingAnchor),
            instructionsLabel.trailingAnchor.constraint(equalTo: trailingAnchor),
            
            captureBiometricButton.bottomAnchor.constraint(equalTo: cancelButton.topAnchor, constant: -BottomButtonsBetween),
            captureBiometricButton.heightAnchor.constraint(equalToConstant: BottomButtonsHeight),
            captureBiometricButton.leadingAnchor.constraint(equalTo: leadingAnchor),
            captureBiometricButton.trailingAnchor.constraint(equalTo: trailingAnchor),
            
            cancelButton.heightAnchor.constraint(equalToConstant: BottomButtonsHeight),
            cancelButton.leadingAnchor.constraint(equalTo: leadingAnchor),
            cancelButton.trailingAnchor.constraint(equalTo: trailingAnchor),
            cancelButton.bottomAnchor.constraint(equalTo: safeAreaLayoutGuide.bottomAnchor),
        ])
    }
    
    private func createLabel() -> UILabel {
        let newLabel = UILabel(frame: .zero)
        
        newLabel.translatesAutoresizingMaskIntoConstraints = false
        newLabel.numberOfLines = 0
        newLabel.backgroundColor = .clear
        
        return newLabel
    }
    
    private func createButton() -> UIButton {
        let newButton = LCRoundButton(type: .custom)
        
        newButton.translatesAutoresizingMaskIntoConstraints = false
        
        return newButton
    }
    
    @objc
    private func captureBiometricButtonTapped(_ sender: UIButton) {
        captureBiometricButtonHandler?()
    }
    
    @objc
    private func cancelButtonTapped(_ sender: UIButton) {
        cancelButtonHandler?()
    }
        
}
