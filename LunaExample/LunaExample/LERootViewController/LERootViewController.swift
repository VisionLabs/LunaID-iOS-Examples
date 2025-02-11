//
//  AuthViewController.swift
//  LunaExample
//
//  Created by IVAN CHIRKOV on 26.10.2020.
//

import UIKit
import LunaCore
import LunaWeb
import LunaCamera
import OCR

class LERootViewController: UIViewController, UITextFieldDelegate {
    
    private let CommonSideOffset: CGFloat = 16
    private let LogoViewTopOffset: CGFloat = 91
    private let ApplicationTitleFontSize: CGFloat = 24
    private let ApplicationTitleTopOffset: CGFloat = 46
    private let NameTextFieldTopOffset: CGFloat = 78
    private let NameTextFieldHeight: CGFloat = 46
    private let LoginButtonTopOffset: CGFloat = 44
    private let LoginButtonsBetween: CGFloat = 16
    private let LoginButtonsHeight: CGFloat = 44
    private let TabButtonsHeight: CGFloat = 60
    private let TabButtonsSeparatorHeight: CGFloat = 1
    
    private let loginButton = LCRoundButton(type: .custom)
    private let usernameField = LETextField(frame: .zero)
    
    private var configFilePath: String?
    
    private lazy var closeHandler: VoidHandler = { [weak self] in
        guard let self = self else { return }
        self.navigationController?.popViewController(animated: true)
    }
    private lazy var closeToRootHandler: VoidHandler = { [weak self] in
        self?.navigationController?.popToRootViewController(animated: true)
    }

    private func createConfig() -> LCLunaConfiguration {
        if let configFilePath {
           return LCLunaConfiguration(plistFilePath: configFilePath)
        } else {
           return LCLunaConfiguration.userDefaults()
        }
    }
    
    init() {
        if let configFilePath = ProcessInfo.processInfo.environment["TEST_CONFIG_FILE_PATH"],
           !configFilePath.isEmpty {
            self.configFilePath = configFilePath
        }

        super.init(nibName: nil, bundle: nil)
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    override func loadView() {
        super.loadView()
        
        createLayout()
    }
    
    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)

        if UIApplication.isEulaAccepted() == false {
            showEula()
        }
    }

    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        
        navigationController?.setNavigationBarHidden(true, animated: true)
    }
    
    //  MARK: - Handlers -
    
    @objc
    private func showSettings() {
        let settingsVC = LESettingsViewController()
        let navvc = UINavigationController(rootViewController: settingsVC)
        navvc.modalPresentationStyle = .fullScreen
        navvc.navigationBar.isHidden = true
        present(navvc, animated: true)
    }
    
    @objc
    private func showRegistration() {
        launchRegistration()
    }
    
    @objc
    private func showIdentifyVerify(_ sender: UIButton) {
        identifyVerifyScenario()
    }
    
    @objc
    private func validateUserName(_ textField: LETextField) {
        guard let text = textField.text else {
            loginButton.isEnabled = true
            loginButton.setTitle("auth.identify".localized(), for: .normal)
            loginButton.accessibilityIdentifier = "auth_identify"
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
                loginButton.accessibilityIdentifier = nil
            } catch {
                textField.isValid = false
                loginButton.isEnabled = false
                loginButton.setTitle("auth.verify".localized(), for: .normal)
                loginButton.accessibilityIdentifier = nil
            }
        }
    }
    
    @objc
    private func tapOnView() {
        view.endEditing(true)
    }
    
    //  MARK: - UITextFieldDelegate -
    
    func textFieldShouldReturn(_ textField: UITextField) -> Bool {
        guard let text = textField.text else {
            identifyVerifyScenario()
            return true
        }
        
        if text.isEmpty {
            identifyVerifyScenario()
            return true
        }
        
        do {
            try text.isValidAsName()
            identifyVerifyScenario()
            return true
        } catch {
            return false
        }
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
        usernameField.placeholder = "success.user_name".localized()
        usernameField.addTarget(self, action: #selector(validateUserName), for: .editingChanged)
        usernameField.autocorrectionType = .no
        view.addSubview(usernameField)
        
        loginButton.translatesAutoresizingMaskIntoConstraints = false
        loginButton.setTitle("auth.identify".localized(), for: .normal)
        loginButton.accessibilityIdentifier = "auth_identify"
        loginButton.addTarget(self, action: #selector(showIdentifyVerify), for: .touchUpInside)
        loginButton.isEnabled = true
        view.addSubview(loginButton)
        
        let tabButtonsContainer = UIView(frame: .zero)
        tabButtonsContainer.translatesAutoresizingMaskIntoConstraints = false
        tabButtonsContainer.backgroundColor = .clear
        view.addSubview(tabButtonsContainer)
        
        let registrationButton = LETabButton(frame: .zero)
        registrationButton.translatesAutoresizingMaskIntoConstraints = false
        registrationButton.tabIcon = UIImage(named: "add_user_icon")
        registrationButton.tabTitle = "registration.sign_up".localized()
        let showRegistrationGesture = UITapGestureRecognizer(target: self, action: #selector(showRegistration))
        registrationButton.addGestureRecognizer(showRegistrationGesture)
        tabButtonsContainer.addSubview(registrationButton)
        
        let settingsButton = LETabButton(frame: .zero)
        settingsButton.translatesAutoresizingMaskIntoConstraints = false
        settingsButton.tabIcon = UIImage(named: "settings_icon")
        settingsButton.tabTitle = "auth.settings".localized()
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
        let viewController = LEEulaViewController()
        viewController.modalPresentationStyle = .fullScreen
        present(viewController, animated: true, completion: nil)
    }
    
    private func identifyVerifyScenario() {
        if let externalID = usernameField.text, !externalID.isEmpty {
            let config = createConfig()
            let lunaAPI = LunaWeb.APIv6(lunaAccountID: config.lunaAccountID,
                                        lunaServerURL: config.lunaPlatformURL) { _ in
                guard let platformToken = config.platformToken else { return [:] }
                return [APIv6Constants.Headers.authorization.rawValue: platformToken]
            }
            
            let query = GetFacesQuery(externalIDs: [externalID], targets: [.externalID, .faceID])

            lunaAPI.faces.getFaces(query: query) { result in
                DispatchQueue.main.async { [weak self] in
                    switch result {
                    case .success(let response):
                        if response.faces.isEmpty {
                            self?.presentModalError(LEServerError.noUser.what())
                        } else {
                            let faceIDs = response.faces.compactMap { $0.id }
                            self?.launchVerify(faceIDs: faceIDs)
                        }
                    case .failure(let error):
                        self?.presentModalError(error.what())
                    }
                }
            }
        } else {
            launchIdentify()
        }
    }
    
    private func launchRegistration() {
        guard !(navigationController?.topViewController is LERegistrationViewController) 
            else { return }
        let viewController = LERegistrationViewController()
        viewController.configuration = createConfig()
        navigationController?.pushViewController(viewController, animated: true)
    }

    // Identify flow

    private func launchIdentify() {
        guard !(navigationController?.topViewController is LEIdentifyViewController) else { return }
        let config = createConfig()
        let identifyViewController = LEIdentifyViewController(configuration: config)
        identifyViewController.resultBlock = { [weak self] faceResult in
            guard let self, presentedViewController == nil,
                  !(navigationController?.topViewController is LEResultViewController)
            else { return }

            switch faceResult {
            case .success(let face, let bestShot):
                if let face, config.ocrEnabled {
                    launchOCR(scenario: .identification, bestShot, face, closeToRootHandler, nil)
                }
                else {
                    let resultViewController = LEResultViewController()

                    resultViewController.setupResult(success: face?.userData != nil,
                                                     stage: .identify,
                                                     userName: face?.userData)
                    pushResultViewController(resultViewController)
                }
            case .cancel:
                launchFailResultScreen(stage: .identify, closeCompletion: closeToRootHandler)
            }
        }
        navigationController?.pushViewController(identifyViewController, animated: true)
    }
    
    private func launchVerify(faceIDs: [String]) {
        guard !(navigationController?.topViewController is LEIdentifyViewController) else { return }
        let config = createConfig()
        let identifyViewController = LEIdentifyViewController(faceIDs: faceIDs,
                                                              configuration: config)
        identifyViewController.resultBlock = { [weak self] faceResult in
            guard let self, presentedViewController == nil,
                  !(navigationController?.topViewController is LEResultViewController)
            else { return }


            switch faceResult {
            case .success(let face, let bestShot):
                if config.ocrEnabled, let face {
                    launchOCR(scenario: .verification, bestShot, face, closeToRootHandler, nil)
                } else {
                    let resultViewController = LEResultViewController()

                    /// В ответе на верификацию возвращает `face` без полей `userData` и `externalID`
                    resultViewController.setupResult(success: face != nil,
                                                     stage: .verify,
                                                     userName: face?.userData)
                    pushResultViewController(resultViewController)
                }
            case .cancel:
                launchFailResultScreen(stage: .verify, closeCompletion: closeToRootHandler)
            }
        }
        navigationController?.pushViewController(identifyViewController, animated: true)
    }

    // ORC flow

    private func launchOCR(scenario: LEOCRResultsViewController.Scenario,
                           _ bestShot: LunaCore.LCBestShotModel,
                           _ face: APIv6.Face,
                           _ closeBlock: @escaping VoidHandler,
                           _ retryOCRResultHandler: ((OCR.OCRResult?) -> Void)?) {
        guard !(navigationController?.topViewController is LEOCRViewController) else { return }
        let viewController = LEOCRViewController()

        viewController.resultBlock = { [weak self] ocrResult in
            guard let ocrResult = ocrResult else {
                self?.launchFailResultScreen(stage: .ocr,
                                             closeCompletion: closeBlock)
                return
            }

            if let retryOCRResultHandler {
                retryOCRResultHandler(ocrResult)
            } else {
                self?.launchOCRSuccessResultScreen(scenario: scenario, bestShot, face, ocrResult)
            }
        }
        
        navigationController?.pushViewController(viewController, animated: true)
    }

    private func launchOCRSuccessResultScreen(scenario: LEOCRResultsViewController.Scenario,
                                              _ bestShot: LunaCore.LCBestShotModel,
                                              _ face: APIv6.Face,
                                              _ ocrResult: OCR.OCRResult?) {
        guard !(navigationController?.topViewController is LEOCRResultsViewController) else { return }
        let ocrResultsViewController = LEOCRResultsViewController()

        ocrResultsViewController.configureResults(scenario: scenario,
                                                  bestShot,
                                                  ocrResult,
                                                  face)

        ocrResultsViewController.continueButtonHandler = { [weak self] error in
            self?.ocrContinueButtonDidTap(face.userData, error)
        }

        ocrResultsViewController.retryBiometricHandler = { [weak self] retryBestShotHandler in
            self?.launchRetryBiometric(retryBestShotHandler)
        }

        ocrResultsViewController.retryOCRHandler = { [weak self] retryOCRResultHandler in
            guard let self else { return }
            launchOCR(scenario: scenario, bestShot, face, closeHandler, retryOCRResultHandler)
        }

        navigationController?.pushViewController(ocrResultsViewController, animated: true)
    }

    // Retry biometric/ocr

    private func launchRetryBiometric(_ retryBestShotHandler: @escaping (LunaCore.LCBestShotModel?) -> Void) {
        guard !(navigationController?.topViewController is LEIdentifyViewController) else { return }
        let config = createConfig()
        let identifyViewController = LEIdentifyViewController(configuration: config)

        identifyViewController.resultBlock = { [weak self] faceResult in
            guard let self else { return }

            switch faceResult {
            case .success(let face, let bestShot):
                if face != nil {
                    retryBestShotHandler(bestShot)
                } else {
                    launchFailResultScreen(stage: .identify, closeCompletion: closeHandler)
                }
            case .cancel:
                launchFailResultScreen(stage: .identify, closeCompletion: closeToRootHandler)
            }
        }
        
        navigationController?.pushViewController(identifyViewController, animated: true)
    }

    // Result screen

    private func launchFailResultScreen(stage: LEResultViewController.ResultStage,
                                        closeCompletion: @escaping VoidHandler) {
        guard !(navigationController?.topViewController is LEResultViewController) else { return }
        let failedViewController = LEResultViewController()
        failedViewController.setupResult(success: false, stage: stage)
        failedViewController.closeHandler = closeCompletion
        navigationController?.pushViewController(failedViewController, animated: true)
    }

    private func ocrContinueButtonDidTap(_ userName: String?, _ error: Error?) {
        guard !(navigationController?.topViewController is LEResultViewController) else { return }
        let resultViewController = LEResultViewController()
        resultViewController.setupResult(success: error == nil,
                                         stage: .verify,
                                         userName: userName)
        self.pushResultViewController(resultViewController)
    }
    
    private func pushResultViewController(_ viewController: LEResultViewController) {
        viewController.closeHandler = self.closeToRootHandler
        navigationController?.pushViewController(viewController, animated: true)
    }
}
