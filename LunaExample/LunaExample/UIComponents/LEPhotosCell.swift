//
//  LEPhotosCell.swift
//  LunaMobile
//
//  Created by IVAN CHIRKOV on 26.04.2021.
//
//

import UIKit
import LunaCamera

class LEPhotosCell: UITableViewCell {
    
    private let StackSpacing: CGFloat = 16
    
    static let reuseID = "LEPhotosCell"
    
    private let bestShotImageView = UIImageView(frame: .zero)
    private let ocrFaceImageView = UIImageView(frame: .zero)
    
    public var retryBiometricHandler: VoidHandler?
    public var retryOCRHandler: VoidHandler?

    override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
        super.init(style: style, reuseIdentifier: reuseIdentifier)
        
        createLayout()
    }

    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    public func configureCell(_ bestShotImage: UIImage?, _ ocrFaceImage: UIImage?) {
        bestShotImageView.image = bestShotImage
        ocrFaceImageView.image = ocrFaceImage
    }

    private func createStackView(_ viewsList: [UIView]) -> UIStackView {
        let newStackView = UIStackView(arrangedSubviews: viewsList)

        newStackView.translatesAutoresizingMaskIntoConstraints = false
        newStackView.axis = .horizontal
        newStackView.distribution = .fillEqually
        newStackView.spacing = StackSpacing

        return newStackView
    }
    
    private func createLayout() {
        bestShotImageView.contentMode = .scaleAspectFit
        ocrFaceImageView.contentMode = .scaleAspectFit
        let imagesStackView = createStackView([bestShotImageView, ocrFaceImageView])
        imagesStackView.translatesAutoresizingMaskIntoConstraints = false
        contentView.addSubview(imagesStackView)
        
        let retryOCRButton = LCRoundButton(type: .custom)
        retryOCRButton.setTitle("capture_error.retry_id".localized(), for: .normal)
        retryOCRButton.addTarget(self, action: #selector(retryOCRButtonTapped), for: .touchUpInside)

        let retryBiometricButton = LCRoundButton(type: .custom)
        retryBiometricButton.setTitle("capture_error.retry_biometric".localized(), for: .normal)
        retryBiometricButton.addTarget(self, action: #selector(retryBiometricButtonTapped), for: .touchUpInside)

        let buttonsStackView = createStackView([retryBiometricButton, retryOCRButton])
        buttonsStackView.translatesAutoresizingMaskIntoConstraints = false
        contentView.addSubview(buttonsStackView)

        NSLayoutConstraint.activate([
            imagesStackView.heightAnchor.constraint(equalTo: ocrFaceImageView.widthAnchor, multiplier: 210.0 / 164.0),
            imagesStackView.topAnchor.constraint(equalTo: contentView.topAnchor),
            imagesStackView.trailingAnchor.constraint(equalTo: contentView.trailingAnchor),
            imagesStackView.leadingAnchor.constraint(equalTo: contentView.leadingAnchor),
            
            buttonsStackView.topAnchor.constraint(equalTo: imagesStackView.bottomAnchor, constant: StackSpacing),
            buttonsStackView.leadingAnchor.constraint(equalTo: contentView.leadingAnchor),
            buttonsStackView.trailingAnchor.constraint(equalTo: contentView.trailingAnchor),
            buttonsStackView.bottomAnchor.constraint(equalTo: contentView.bottomAnchor)
        ])
    }
    
    @objc
    private func retryOCRButtonTapped(_sender: UIButton) {
        retryOCRHandler?()
    }
    
    @objc
    private func retryBiometricButtonTapped(_sender: UIButton) {
        retryBiometricHandler?()
    }
    
}
