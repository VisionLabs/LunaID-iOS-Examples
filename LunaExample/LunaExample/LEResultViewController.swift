//
//  LASuccessViewController.swift
//  LunaExampleOCR
//
//  Created by michael on 24.10.2023.
//

import UIKit
import LunaCamera

class LEResultViewController: UIViewController {
    
    private let BetweenOffset: CGFloat = 16
    private let FinishButtonHeight: CGFloat = 44

    private let finishButton = LunaCamera.LCRoundButton(type: .custom)
    private let smileView = UIImageView(image: UIImage(named: "success"))
    private let titleLabel = UILabel(frame: .zero)

    public var closeHandler: VoidHandler?
    
    public var resultImageName: String = "success" {
        didSet {
            smileView.image = UIImage(named: resultImageName)
        }
    }

    public var resultTitle: String = "success" {
        didSet {
            titleLabel.text = resultTitle
        }
    }

    override func loadView() {
        super.loadView()
        
        createLayout()
    }
    
    private func createLayout() {
        view.backgroundColor = .white
        
        smileView.translatesAutoresizingMaskIntoConstraints = false
        view.addSubview(smileView)
        
        titleLabel.numberOfLines = 0
        titleLabel.translatesAutoresizingMaskIntoConstraints = false
        titleLabel.textAlignment = .center
        titleLabel.font = Fonts.etelka_20
        view.addSubview(titleLabel)
        
        finishButton.translatesAutoresizingMaskIntoConstraints = false
        finishButton.setTitle("finish.button".localized(), for: .normal)
        finishButton.addTarget(self, action: #selector(finishButtonTapped), for: .touchUpInside)
        finishButton.isEnabled = true
        view.addSubview(finishButton)

        NSLayoutConstraint.activate([
            smileView.centerXAnchor.constraint(equalTo: view.centerXAnchor),
            smileView.centerYAnchor.constraint(equalTo: view.centerYAnchor),
            
            titleLabel.topAnchor.constraint(equalTo: smileView.bottomAnchor, constant: BetweenOffset),
            titleLabel.leadingAnchor.constraint(equalTo: view.leadingAnchor),
            titleLabel.trailingAnchor.constraint(equalTo: view.trailingAnchor),
            
            finishButton.bottomAnchor.constraint(equalTo: view.safeAreaLayoutGuide.bottomAnchor),
            finishButton.leadingAnchor.constraint(equalTo: view.leadingAnchor, constant: BetweenOffset),
            finishButton.trailingAnchor.constraint(equalTo: view.trailingAnchor, constant: -BetweenOffset),
            finishButton.heightAnchor.constraint(equalToConstant: FinishButtonHeight),
        ])
    }
    
    @objc
    private func finishButtonTapped() {
        closeHandler?()
    }
    
}
