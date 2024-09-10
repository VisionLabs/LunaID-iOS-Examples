//
//  LEInputVC.swift
//  LunaExample
//
//  Created by Kirill Zhadaev on 03.09.2024.
//

import UIKit

final class LEInputVC: UIViewController {

    private let InputTextFieldHeight: CGFloat = 46
    private let CommonSideOffset: CGFloat = 16
    private var text: String?

    private lazy var inputField: LETextField = {
        let textFiled = LETextField(frame: .zero)
        textFiled.translatesAutoresizingMaskIntoConstraints = false
        textFiled.placeholder = "input.placeholder_enter".localized()
        textFiled.addTarget(self, action: #selector(textDidChanged), for: .editingChanged)
        textFiled.autocorrectionType = .no
        return textFiled
    }()

    private lazy var doneButton: UIBarButtonItem = {
        UIBarButtonItem(title: "done.button".localized(),
                        style: .done,
                        target: self,
                        action: #selector(doneButtonDidTap))
    }()

    var valueChangedHandler: ((String?) -> Void)?

    override func viewWillDisappear(_ animated: Bool) {
        navigationController?.navigationBar.isHidden = true
    }

    override func viewWillAppear(_ animated: Bool) {
        navigationController?.navigationBar.isHidden = false
    }

    init(initialText: String?) {
        super.init(nibName: nil, bundle: nil)
        self.inputField.text = initialText
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    override func loadView() {
        super.loadView()

        createLayout()
    }

    //  MARK: - Handlers -

    @objc
    private func tapOnView() {
        view.endEditing(true)
    }

    @objc
    private func doneButtonDidTap() {
        navigationController?.popViewController() { [weak self] in
            guard let text = self?.text, !text.isEmpty else {
                self?.valueChangedHandler?(nil)
                return
            }

            self?.valueChangedHandler?(text)
        }
    }

    @objc
    private func textDidChanged() {
        text = inputField.text
    }

    //  MARK: - Routine -

    private func createLayout() {
        view.backgroundColor = .white
        let tapGesture = UITapGestureRecognizer(target: self, action: #selector(tapOnView))
        view.addGestureRecognizer(tapGesture)

        view.addSubview(inputField)
        navigationItem.setRightBarButton(doneButton, animated: false)

        NSLayoutConstraint.activate([
            inputField.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor, constant: CommonSideOffset),
            inputField.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: CommonSideOffset),
            inputField.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -CommonSideOffset),
            inputField.heightAnchor.constraint(equalToConstant: InputTextFieldHeight),
        ])
    }
}
