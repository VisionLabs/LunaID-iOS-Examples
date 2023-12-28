//
//  AuthViewController.swift
//  LunaExample
//
//  Created by IVAN CHIRKOV on 26.10.2020.
//

import UIKit
import LunaAuth
import LunaCore
import LunaWeb
import LunaCamera
import CoreLocation

class AuthViewController: UIViewController, UITextFieldDelegate, LunaIdentifyFlowDelegate, LunaVerifyFlowDelegate {
    
    private let CommonSideOffset: CGFloat = 16
    private let LogoViewTopOffset: CGFloat = 91
    private let ApplicationTitleFontSize: CGFloat = 24
    private let ApplicationTitleTopOffset: CGFloat = 46
    private let NameTextFieldTopOffset: CGFloat = 78
    private let NameTextFieldHeight: CGFloat = 46
    private let LoginButtonTopOffset: CGFloat = 44
    private let LoginButtonsHeight: CGFloat = 44
    private let TabButtonsHeight: CGFloat = 60
    private let TabButtonsSeparatorHeight: CGFloat = 1

    private let loginButton = LCRoundButton(type: .custom)
    private let usernameField = TextField(frame: .zero)
    
    private var currentFlow: LunaFlowProtocol?
    private let configuration: LCLunaConfiguration = LCLunaConfiguration.defaultConfig()
    private let legacyStoryboard = UIStoryboard(name: "Main", bundle: Bundle(for: AuthViewController.self))

    private lazy var lunaAPI: LunaWeb.APIv6 = {
        APIv6(lunaAccountID: configuration.lunaAccountID,
              lunaServerURL: configuration.lunaServerURL,
              additionalHeaders: nil)
    }()
    
    override func loadView() {
        super.loadView()
        
        configuration.ocrEnabled = false
        configuration.bestShotConfiguration.numberOfBestShots = 2
        createLayout()
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        if UIApplication.isEulaAccepted() == false {
            showEula()
        }
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        
        navigationController?.setNavigationBarHidden(true, animated: true)
    }
    
    @objc
    private func validateUserName(_ textField: TextField) {
        guard let text = textField.text else {
            loginButton.isEnabled = true
            loginButton.setTitle("auth.identify".localized(), for: .normal)
            return
        }
        
        if text.isEmpty {
            loginButton.isEnabled = true
            textField.isValid = true
            loginButton.setTitle("auth.identify".localized(), for: .normal)
        } else {
            do {
                let isValid = try text.isValidAsName()
                textField.isValid = isValid
                loginButton.isEnabled = isValid
                loginButton.setTitle("auth.verify".localized(), for: .normal)
            } catch {
                textField.isValid = false
                loginButton.isEnabled = false
                loginButton.setTitle("auth.verify".localized(), for: .normal)
            }
        }
    }
    
    //  MARK: - Handlers -
    
    @objc
    private func tapOnView() {
        view.endEditing(true)
    }
    
    @objc
    private func showSettings() {
        let viewController = SettingsViewController()
        viewController.configuration = configuration
        
        let navvc = UINavigationController(rootViewController: viewController)
        navvc.modalPresentationStyle = .fullScreen
        navvc.navigationBar.isHidden = true
        present(navvc, animated: true)
    }
    
    @objc
    private func showRegistration() {
        navigationController?.pushViewController(RegistrationViewController(), animated: true)
    }
    
    @objc
    private func login(_ sender: UIButton) {
        showLogin()
    }
    
    //  MARK: - UITextFieldDelegate -
    
    func textFieldShouldReturn(_ textField: UITextField) -> Bool {
        guard let text = textField.text else {
            showLogin()
            return true
        }
        
        if text.isEmpty {
            showLogin()
            return true
        }
        
        do {
            try text.isValidAsName()
            showLogin()
            return true
        } catch {
            return false
        }
    }

    //  MARK: - LunaIdentifyFlowDelegate -

    func identify() {
        let newFlow = LunaFlowBuilder.identify(configuration: configuration,
                                                    delegate: self)
        currentFlow = newFlow
        let viewController = newFlow.currentViewController()
        viewController.modalPresentationStyle = .fullScreen
        present(viewController, animated: true)
    }

    func identificationCompleted(candidate: LunaWeb.APIv6.Face, location: CLLocation?) {
        dismiss(animated: false)
        presentSuccess(candidate, location: location, isIdentify: true)
    }

    func identificationCanceled(with error: Error?, location: CLLocation?) {
        dismiss(animated: false)
        canceled(with: error, location: location)
    }

    //  MARK: - LunaVerifyFlowDelegate -

    func verify(faceID: String) {
        let newFlow = LunaFlowBuilder.verify(faceID: faceID, configuration: configuration, delegate: self)
        currentFlow = newFlow
        let viewController = newFlow.currentViewController()
        viewController.modalPresentationStyle = .fullScreen
        present(viewController, animated: true)
    }

    public func verificationCompleted(candidate: LunaWeb.APIv6.Face, location: CLLocation?) {
        dismiss(animated: false)
        presentSuccess(candidate, location: location, isIdentify: false)
    }

    public func verificationCanceled(with error: Error?, location: CLLocation?) {
        dismiss(animated: false)
        canceled(with: error, location: location)
    }

    //  MARK: - Routine -

    private func createLayout() {
        navigationController?.navigationBar.topItem?.title = ""
        let tapGesture = UITapGestureRecognizer(target: self, action: #selector(tapOnView))
        view.addGestureRecognizer(tapGesture)
        
        let logoView = UIImageView(image: UIImage(named: "logo"))
        logoView.translatesAutoresizingMaskIntoConstraints = false
        view.addSubview(logoView)
        
        let applicationTitleLabel = UILabel(frame: .zero)
        applicationTitleLabel.text = "LUNA ID Demo"
        applicationTitleLabel.textAlignment = .center
        applicationTitleLabel.font = UIFont.systemFont(ofSize: ApplicationTitleFontSize)
        applicationTitleLabel.translatesAutoresizingMaskIntoConstraints = false
        view.addSubview(applicationTitleLabel)
        
        usernameField.translatesAutoresizingMaskIntoConstraints = false
        usernameField.delegate = self
        usernameField.placeholder = "Username"
        usernameField.addTarget(self, action: #selector(validateUserName), for: .editingChanged)
        usernameField.autocorrectionType = .no
        view.addSubview(usernameField)
        
        loginButton.translatesAutoresizingMaskIntoConstraints = false
        loginButton.setTitle("Identification", for: .normal)
        loginButton.addTarget(self, action: #selector(login), for: .touchUpInside)
        loginButton.isEnabled = true
        view.addSubview(loginButton)
        
        let tabButtonsContainer = UIView(frame: .zero)
        tabButtonsContainer.translatesAutoresizingMaskIntoConstraints = false
        tabButtonsContainer.backgroundColor = .clear
        view.addSubview(tabButtonsContainer)
        
        let registrationButton = TabButton(frame: .zero)
        registrationButton.translatesAutoresizingMaskIntoConstraints = false
        registrationButton.tabIcon = UIImage(named: "add_user_icon")
        registrationButton.tabTitle = "New User"
        let showRegistrationGesture = UITapGestureRecognizer(target: self, action: #selector(showRegistration))
        registrationButton.addGestureRecognizer(showRegistrationGesture)
        tabButtonsContainer.addSubview(registrationButton)
        
        let settingsButton = TabButton(frame: .zero)
        settingsButton.translatesAutoresizingMaskIntoConstraints = false
        settingsButton.tabIcon = UIImage(named: "settings_icon")
        settingsButton.tabTitle = "Settings"
        let showSettingsGesture = UITapGestureRecognizer(target: self, action: #selector(showSettings))
        settingsButton.addGestureRecognizer(showSettingsGesture)
        tabButtonsContainer.addSubview(settingsButton)
        
        let separatorView = UIView(frame: .zero)
        separatorView.backgroundColor = .lunaLightGray()
        separatorView.translatesAutoresizingMaskIntoConstraints = false
        tabButtonsContainer.addSubview(separatorView)
        
        NSLayoutConstraint.activate([
            logoView.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor, constant: LogoViewTopOffset),
            logoView.centerXAnchor.constraint(equalTo: view.safeAreaLayoutGuide.centerXAnchor),
            
            applicationTitleLabel.topAnchor.constraint(equalTo: logoView.bottomAnchor, constant: ApplicationTitleTopOffset),
            applicationTitleLabel.leadingAnchor.constraint(equalTo: view.leadingAnchor),
            applicationTitleLabel.trailingAnchor.constraint(equalTo: view.trailingAnchor),
            
            usernameField.topAnchor.constraint(equalTo: applicationTitleLabel.bottomAnchor, constant: NameTextFieldTopOffset),
            usernameField.leadingAnchor.constraint(equalTo: view.leadingAnchor, constant: CommonSideOffset),
            usernameField.trailingAnchor.constraint(equalTo: view.trailingAnchor, constant: -CommonSideOffset),
            usernameField.heightAnchor.constraint(equalToConstant: NameTextFieldHeight),
            
            loginButton.topAnchor.constraint(equalTo: usernameField.bottomAnchor, constant: LoginButtonTopOffset),
            loginButton.leadingAnchor.constraint(equalTo: view.leadingAnchor, constant: CommonSideOffset),
            loginButton.trailingAnchor.constraint(equalTo: view.trailingAnchor, constant: -CommonSideOffset),
            loginButton.heightAnchor.constraint(equalToConstant: LoginButtonsHeight),

            registrationButton.leadingAnchor.constraint(equalTo: tabButtonsContainer.leadingAnchor),
            registrationButton.trailingAnchor.constraint(equalTo: tabButtonsContainer.centerXAnchor),
            registrationButton.bottomAnchor.constraint(equalTo: tabButtonsContainer.bottomAnchor),
            
            settingsButton.leadingAnchor.constraint(equalTo: tabButtonsContainer.centerXAnchor),
            settingsButton.trailingAnchor.constraint(equalTo: tabButtonsContainer.trailingAnchor),
            settingsButton.bottomAnchor.constraint(equalTo: tabButtonsContainer.bottomAnchor),
            
            separatorView.leadingAnchor.constraint(equalTo: tabButtonsContainer.leadingAnchor),
            separatorView.trailingAnchor.constraint(equalTo: tabButtonsContainer.trailingAnchor),
            separatorView.topAnchor.constraint(equalTo: tabButtonsContainer.topAnchor),
            separatorView.heightAnchor.constraint(equalToConstant: TabButtonsSeparatorHeight),

            tabButtonsContainer.heightAnchor.constraint(equalToConstant: TabButtonsHeight),
            tabButtonsContainer.bottomAnchor.constraint(equalTo: view.safeAreaLayoutGuide.bottomAnchor),
            tabButtonsContainer.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor),
            tabButtonsContainer.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor),
        ])
    }
    
    private func showEula() {
        let controller = legacyStoryboard.instantiateViewController(withIdentifier: "EulaViewController")
        controller.modalPresentationStyle = .fullScreen
        present(controller, animated: true, completion: nil)
    }
    
    private func showLogin() {
        if let userName = usernameField.text, !userName.isEmpty {
            let query = GetFacesQuery(userData: userName, targets: [.userData, .faceID])
            lunaAPI.faces.getFaces(query: query) { [weak self] result in
                DispatchQueue.main.async {
                    switch result {
                    case .success(let response):
                        if let faceID = response.faces.first(where: { $0.userData == userName })?.id {
                            self?.verify(faceID: faceID)
                        } else {
                            self?.presentError(ServerError.noUser.what())
                        }
                    case .failure(let error):
                        self?.presentError(error.what())
                    }
                }
            }
        } else {
            identify()
        }
    }
    
    private func canceled(with error: Error?, location: CLLocation?) {
        guard let error = error else { return }
        if case AuthError.identificationError = error {
            presentFail(isIdentify: true, location: location)
        }
        else if case AuthError.verificationError = error {
            presentFail(isIdentify: false, location: location)
        }
        else if case LMCameraError.accessStatus = error {
            let action = AlertAction(title: "common.open_app_settings".localized()) {
                UIApplication.openAppSettings()
            }
            presentError(error.what(), actions: [action])
        } else {
            presentError(error.what())
        }
    }
    
    private func presentSuccess(_ candidate: LunaWeb.APIv6.Face, location: CLLocation?, isIdentify: Bool) {
        guard let controller = legacyStoryboard.instantiateViewController(withIdentifier: "SuccessViewController") as? SuccessViewController else { return }
        controller.candidate = candidate
        controller.location = location
        controller.isIdentify = isIdentify
        navigationController?.pushViewController(controller, animated: true)
    }
    
    private func presentFail(isIdentify: Bool, location: CLLocation?) {
        guard let controller = legacyStoryboard.instantiateViewController(withIdentifier: "FailViewController") as? FailViewController else { return }
        controller.isIdentify = isIdentify
        controller.location = location
        navigationController?.pushViewController(controller, animated: true)
    }
    
}
