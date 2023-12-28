//
//  IntroductionView.swift
//  Liveness Demo
//
//  Created by Иван Казанцев on 24.09.2023.
//

import UIKit

final class IntroductionView: UIView {

    private let textView: UITextView = {
        let view = UITextView(frame: .zero)
        view.translatesAutoresizingMaskIntoConstraints = false
        view.isEditable = false
        view.isScrollEnabled = false
        view.font = UIFont.etelka_16
        view.text = "Recommendations: 1. Face should be turned towards the camera. Head should not be tilted or turned.\n 2. Face should be well-lit. It should not be occluded by foreign objects.\n 3. Face should be in the center of the screen within its borders.\n  4. Face should not be too far from camera.\n 5. There should be a single face in the frame. There should not be any other people or pictures of people in the background."
        return view
    }()

    private let acceptButton: UIButton = {
        let button = UIButton()
        button.translatesAutoresizingMaskIntoConstraints = false
        button.layer.cornerRadius = 15
        button.backgroundColor = UIColor.resolutionBlue
        button.setTitle("Accept", for: .normal)
        button.titleLabel?.font = UIFont.etelka_15
        return button
    }()

    var closeBlock: (() -> Void)?

    init() {
        super.init(frame: .zero)
        backgroundColor = .white
        prepareLayout()
        prepareActions()
    }

    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    @objc private func acceptButtonWasTapped() {
        closeBlock?()
    }
}

// MARK: - Closable

extension IntroductionView: Closable {

    func configure(withCloseBlock closeBlock: @escaping () -> Void) {
        self.closeBlock = closeBlock
    }
}

private extension IntroductionView {

    func prepareLayout() {
        addSubview(acceptButton)
        addSubview(textView)
        NSLayoutConstraint.activate([
            acceptButton.bottomAnchor.constraint(equalTo: safeAreaLayoutGuide.bottomAnchor, constant: -75),
            acceptButton.leadingAnchor.constraint(equalTo: safeAreaLayoutGuide.leadingAnchor, constant: 25),
            acceptButton.trailingAnchor.constraint(equalTo: safeAreaLayoutGuide.trailingAnchor, constant: -25),
            acceptButton.heightAnchor.constraint(equalToConstant: 55),

            textView.leadingAnchor.constraint(equalTo: acceptButton.leadingAnchor),
            textView.trailingAnchor.constraint(equalTo: acceptButton.trailingAnchor),
            textView.topAnchor.constraint(equalTo: safeAreaLayoutGuide.topAnchor, constant: 25),
            textView.bottomAnchor.constraint(equalTo: acceptButton.topAnchor, constant: -25)
        ])
    }

    func prepareActions() {
        acceptButton.addTarget(self, action: #selector(acceptButtonWasTapped), for: .touchUpInside)
    }
}
