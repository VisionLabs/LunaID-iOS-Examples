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
    
    private let configuration: LCLunaConfiguration = LCLunaConfiguration()
    
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
        let navvc = UINavigationController(rootViewController: LESettingsViewController())
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
        if let userName = usernameField.text,
           !userName.isEmpty {
            let query = GetFacesQuery(userData: userName, targets: [.userData, .faceID])
            let lunaAPI = LunaWeb.APIv6(lunaAccountID: configuration.lunaAccountID,
                                        lunaServerURL: configuration.lunaServerURL,
                                        additionalHeaders: nil)
            lunaAPI.faces.getFaces(query: query) { result in
                DispatchQueue.main.async { [weak self] in
                    switch result {
                    case .success(let response):
                        if let faceID = response.faces.first(where: { $0.userData == userName })?.id {
                            self?.launchVerify(faceID: faceID)
                        } else {
                            self?.presentModalError(LEServerError.noUser.what())
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
        let viewController = LERegistrationViewController()
        viewController.configuration = configuration
        navigationController?.pushViewController(viewController, animated: true)
    }

    // Identify flow

    private func launchIdentify() {
        let identifyViewController = createIdentifyViewController()
        identifyViewController.resultBlock = { [weak self] face, bestShot in
            guard let self = self else { return }
            let resultViewController = LEResultViewController()
            if let name = face?.userData {
                if (self.configuration.ocrEnabled) {
                    let closeHandler: VoidHandler = { [weak self] in
                        self?.navigationController?.popToRootViewController(animated: true)
                    }

                    self.launchOCR(bestShot, name, closeHandler)
                    return
                }
                else {
                    resultViewController.resultTitle = name + "\n\n"
                    resultViewController.resultTitle += "success.identify_result".localized()
                    resultViewController.resultImageName = "success"
                }
            }
            else {
                if (self.configuration.bestShotConfiguration.livenessType != .byPhoto) {
                    resultViewController.resultTitle = "\n\n"
                    resultViewController.resultTitle += "success.identify_result".localized()
                    resultViewController.resultImageName = "success"
                }
                else {
                    resultViewController.resultTitle = "fail.identify_result".localized()
                    resultViewController.resultImageName = "fail"
                }
            }
            
            resultViewController.closeHandler = { [weak self] in
                self?.navigationController?.popToRootViewController(animated: true)
            }
            self.navigationController?.pushViewController(resultViewController, animated: true)
        }
        navigationController?.pushViewController(identifyViewController, animated: true)
    }
    
    private func launchVerify(faceID: String) {
        let identifyViewController = createIdentifyViewController()
        identifyViewController.resultBlock = { [weak self] face, bestShot in
            guard let self = self else { return }
            let resultViewController = LEResultViewController()
            resultViewController.resultTitle = "fail.verify_result".localized()
            resultViewController.resultImageName = "fail"
            if let detectedFaceID = face?.id {
                if (detectedFaceID == faceID) {
                    if (self.configuration.ocrEnabled) {
                        let closeHandler: VoidHandler = { [weak self] in
                            self?.navigationController?.popToRootViewController(animated: true)
                        }

                        self.launchOCR(bestShot, face?.userData, closeHandler)
                        return
                    }
                    else {
                        resultViewController.resultTitle = (face?.userData ?? "") + "\n\n"
                        resultViewController.resultTitle += "success.verify_result".localized()
                        resultViewController.resultImageName = "success"
                    }
                }
                else {
                    resultViewController.resultTitle = face?.userData ?? "" + "\n\n"
                    resultViewController.resultTitle += "fail.verify_result".localized()
                    resultViewController.resultImageName = "fail"
                }
            }
            
            resultViewController.closeHandler = { [weak self] in
                self?.navigationController?.popToRootViewController(animated: true)
            }
            self.navigationController?.pushViewController(resultViewController, animated: true)
        }
        navigationController?.pushViewController(identifyViewController, animated: true)
    }
    
    private func createIdentifyViewController() -> LEIdentifyViewController {
        let identifyViewController = LEIdentifyViewController()
        identifyViewController.configuration = configuration
        
        return identifyViewController
    }

    // ORC flow

    private func launchOCR(_ bestShot: LunaCore.LCBestShot,
                           _ userName: String?,
                           _ closeHandler: @escaping VoidHandler) {
        let viewController = LEOCRViewController()

        viewController.resultBlock = { [weak self] ocrResult in
            guard let ocrResult = ocrResult else {
                self?.launchFailResultScreen(resultTitle: "ocr.failed".localized(),
                                             closeHandler: closeHandler)
                return
            }

            self?.launchOCRSuccessResultScreen(bestShot, userName, ocrResult)
        }

        navigationController?.pushViewController(viewController, animated: true)
    }

    private func launchOCRSuccessResultScreen(_ bestShot: LunaCore.LCBestShot,
                                              _ userName: String?,
                                              _ ocrResult: OCR.OCRResult?) {
        let ocrResultsViewController = LEOCRResultsViewController()

        ocrResultsViewController.configureResults(bestShot,
                                                  ocrResult,
                                                  "buttons.do_actions.verify".localized())

        ocrResultsViewController.continueButtonHandler = { [weak self] error in
            self?.ocrContinueButtonDidTap(userName, error)
        }

        ocrResultsViewController.retryBiometricHandler = { [weak self] in
            self?.launchRetryBiometric(ocrResult)
        }

        ocrResultsViewController.retryOCRHandler = { [weak self] in
            self?.launchRetryOCR(bestShot, userName)
        }

        navigationController?.pushViewController(ocrResultsViewController, animated: true)
    }

    // Retry biometric/ocr

    private func launchRetryBiometric(_ ocrResult: OCR.OCRResult?) {
        let identifyViewController = createIdentifyViewController()

        identifyViewController.resultBlock = { [weak self] face, bestShot in
            // Close IdentifyViewController
            self?.navigationController?.popViewController(animated: true) {
                if let name = face?.userData {
                    self?.launchOCRSuccessResultScreen(bestShot, name, ocrResult)
                } else {
                    self?.launchFailResultScreen(resultTitle: "fail.identify_result".localized()) {
                        self?.navigationController?.popViewController(animated: true)
                    }
                }
            }
        }

        navigationController?.pushViewController(identifyViewController, animated: true)
    }

    private func launchRetryOCR(_ bestShot: LunaCore.LCBestShot, _ userName: String?) {
        let closeHandler: VoidHandler = { [weak self] in
            self?.navigationController?.popViewController(animated: true)
        }

        launchOCR(bestShot, userName, closeHandler)
    }

    // Result screen

    private func launchFailResultScreen(resultTitle: String, closeHandler: @escaping VoidHandler) {
        let failedViewController = LEResultViewController()
        failedViewController.resultTitle = resultTitle
        failedViewController.resultImageName = "fail"
        failedViewController.closeHandler = closeHandler
        navigationController?.pushViewController(failedViewController, animated: true)
    }

    private func ocrContinueButtonDidTap(_ userName: String?, _ error: Error?) {
        let resultViewController = LEResultViewController()
        resultViewController.resultTitle =  "fail.verify_result".localized()
        resultViewController.resultImageName = "fail"

        if error != nil {
            resultViewController.resultTitle = (userName ?? "") + "\n\n"
            resultViewController.resultTitle +=  "fail.verify_result".localized()
            resultViewController.resultImageName = "fail"
        }
        else {
            resultViewController.resultTitle = (userName ?? "") + "\n\n"
            resultViewController.resultTitle += "success.verify_result".localized()
            resultViewController.resultImageName = "success"
        }

        resultViewController.closeHandler = { [weak self] in
            self?.navigationController?.popToRootViewController(animated: true)
        }

        navigationController?.pushViewController(resultViewController, animated: true)
    }
    
}
