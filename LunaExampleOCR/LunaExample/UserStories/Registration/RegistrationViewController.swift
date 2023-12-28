//
//  RegistrationViewController.swift
//  LunaExample
//
//  Created by IVAN CHIRKOV on 26.10.2020.
//

import UIKit
import LunaAuth
import LunaCore
import LunaCamera
import LunaWeb
import CoreLocation

class RegistrationViewController: UIViewController, LunaRegistrationFlowDelegate, UITextFieldDelegate {

    private let UserNameTextFieldHeight: CGFloat = 46
    private let CommonSideOffset: CGFloat = 16

    private let usernameField = TextField(frame: .zero)
    private let usernameReasonLabel = UILabel(frame: .zero)
    
    private var currentFlow: LunaFlowProtocol?
    
    private lazy var signUpButton: UIBarButtonItem = {
        UIBarButtonItem(title: "registration.sign_up".localized(),
                        style: .done,
                        target: self,
                        action: #selector(signUP))
    }()
    
    private lazy var activityView: ActivityIndicator = ActivityIndicator(image: UIImage(named: "activity_indicator"))
    private lazy var activityItem: UIBarButtonItem = {
        activityView.frame.size = CGSize(width: 30, height: 30)
        activityView.start()
        let view = UIView(frame: CGRect(x: 0, y: 0, width: 36, height: 30))
        view.addSubview(activityView)
        let item = UIBarButtonItem(customView: view)
        activityView.center = view.center
        return item
    }()
    
    private let configuration = LCLunaConfiguration.defaultConfig()
    private lazy var lunaAPI: LunaWeb.APIv6 = {
        APIv6(lunaAccountID: configuration.lunaAccountID,
              lunaServerURL: configuration.lunaServerURL,
              additionalHeaders: nil)
    }()
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        
        navigationController?.setNavigationBarHidden(false, animated: true)
    }
    
    override func loadView() {
        super.loadView()
        
        configuration.locationEnabled = false
        configuration.bestShotConfiguration.numberOfBestShots = 2
        createLayout()
    }
    
    private func createLayout() {
        view.backgroundColor = .white
        let tapGesture = UITapGestureRecognizer(target: self, action: #selector(tapOnView))
        view.addGestureRecognizer(tapGesture)
        
        usernameField.translatesAutoresizingMaskIntoConstraints = false
        usernameField.delegate = self
        usernameField.placeholder = "Username"
        usernameField.addTarget(self, action: #selector(validateUserName), for: .editingChanged)
        usernameField.autocorrectionType = .no
        view.addSubview(usernameField)
        
        usernameReasonLabel.translatesAutoresizingMaskIntoConstraints = false
        usernameReasonLabel.isHidden = true
        view.addSubview(usernameReasonLabel)
        
        signUpButton.isEnabled = false
        navigationItem.setRightBarButton(signUpButton, animated: false)

        NSLayoutConstraint.activate([
            usernameField.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor, constant: CommonSideOffset),
            usernameField.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: CommonSideOffset),
            usernameField.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -CommonSideOffset),
            usernameField.heightAnchor.constraint(equalToConstant: UserNameTextFieldHeight),
            
            usernameReasonLabel.topAnchor.constraint(equalTo: usernameField.bottomAnchor, constant: CommonSideOffset),
            usernameReasonLabel.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: CommonSideOffset),
            usernameReasonLabel.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -CommonSideOffset)
        ])
    }
    
    //  MARK: - Handlers -
    
    @objc
    private func tapOnView() {
        view.endEditing(true)
    }
    
    @objc
    private func signUP() {
        activityView.start()
        showActivity()
        
        usernameReasonLabel.isHidden = true
        usernameField.isValid = true
        
        let query = GetFacesQuery(userData: usernameField.text, externalIDs: nil, targets: [.faceID, .userData])
        
        lunaAPI.faces.getFaces(query: query) { result in
            DispatchQueue.main.async { [weak self] in
                guard let userName = self?.usernameField.text else {
                    return
                }
                
                switch result {
                case .success(let facesResponse):
                    let takenUserNames = facesResponse.faces.compactMap({ $0.userData })
                    // Проверяем занято ли имя пользователя на сервере Luna
                    if takenUserNames.contains(userName) {
                        self?.usernameReasonLabel.isHidden = false
                        self?.usernameField.isValid = false
                        self?.usernameReasonLabel.text = ServerError.userNameAlreadyTaken.what()
                    } else {
                        self?.registration(externalID: nil, userName: userName)
                    }
                    self?.showSignUPButton()
                case .failure(let error):
                    self?.showSignUPButton()
                    self?.presentError(error.what())
                }
            }
        }
    }
    
    @objc
    private func validateUserName(_ textField: TextField) {
        guard let text = textField.text else {
            signUpButton.isEnabled = false
            return
        }
        
        if text.isEmpty {
            signUpButton.isEnabled = false
            return
        }
        
        do {
            let isValid = try textField.text.isValidAsName()
            textField.isValid = isValid
            usernameReasonLabel.isHidden = isValid
            signUpButton.isEnabled = isValid
        } catch {
            textField.isValid = false
            signUpButton.isEnabled = false
            usernameReasonLabel.isHidden = false
            usernameReasonLabel.text = error.what()
        }
    }
    
    
    //  MARK: - Routine -
    
    private func showSignUPButton() {
        navigationItem.setRightBarButton(signUpButton, animated: true)
    }
    
    private func showActivity() {
        navigationItem.setRightBarButton(activityItem, animated: true)
    }
    
    private func registration(externalID: String?, userName: String) {
        let newFlow = LunaFlowBuilder.registration(configuration: configuration,
                                                   delegate: self,
                                                   externalID: externalID,
                                                   userData: userName)
        currentFlow = newFlow
        let viewController = newFlow.currentViewController()
        viewController.modalPresentationStyle = .fullScreen
        present(viewController, animated: true)
    }
    
    private func showSuccess(location: CLLocation?) {
        var title = "registration.successful".localized()
        if let location = location {
            title += ". Location: \(location.description)"
        }
        let alert = UIAlertController(title: title, message: nil, preferredStyle: .alert)
        alert.addAction(.init(title: "OK", style: .default, handler: { [weak self] _ in
            self?.navigationController?.popViewController(animated: true)
        }))
        DispatchQueue.main.async {
            self.present(alert, animated: true, completion: nil)
        }
    }
    
    //  MARK: - LunaRegistrationFlowDelegate -
    
    public func registrationCompleted(faceID: String, externalID: String?, location: CLLocation?) {
        dismiss(animated: false)
        showSignUPButton()
        showSuccess(location: location)
    }
    
    public func registrationCanceled(with error: Error?, location: CLLocation?) {
        dismiss(animated: false)
        showSignUPButton()
        guard let error = error else { return }
        if case LMCameraError.accessStatus = error {
            let action = AlertAction(title: "common.open_app_settings".localized()) {
                UIApplication.openAppSettings()
            }
            presentError(error.what(), actions: [action])
        } else {
            presentError(error.what())
        }
    }
    
    //  MARK: - UITextFieldDelegate -
    
    public func textFieldShouldReturn(_ textField: UITextField) -> Bool {
        do {
            let isValid = try textField.text.isValidAsName()
            signUP()
            return isValid
        } catch {
            return false
        }
    }
    
}
