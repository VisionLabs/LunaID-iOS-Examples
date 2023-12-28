//
//  LivenessViewController.swift
//  Liveness Demo
//
//  Created by Иван Казанцев on 24.09.2023.
//

import UIKit
import LunaCamera
import LunaWeb
import LunaCore

final class LivenessViewController: UIViewController {

    /// Controller state
    enum State {
        /// Introduction view is active
        case intro
        /// Liveness scenario is active
        case liveness
    }

    // MARK: - Initialization

    init() {
        super.init(nibName: nil, bundle: nil)
        change(state: .intro)
        introductionView.configure { [weak self] in
            guard let self else { return }
            self.state = .liveness
        }
        configureLuna()
        addActions()
    }

    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    // MARK: - State

    private var state: State = .intro {
        didSet {
            change(state: state)
        }
    }

    // MARK: - Views

    private let introductionView: UIView & Closable = {
        let view = IntroductionView()
        view.translatesAutoresizingMaskIntoConstraints = false
        return view
    }()

    private let logo: UIImageView = {
        let image = UIImageView()
        image.translatesAutoresizingMaskIntoConstraints = false
        image.image = UIImage(named: "logo")
        return image
    }()

    private let appName: UILabel = {
        let label = UILabel()
        label.translatesAutoresizingMaskIntoConstraints = false
        label.text = "Liveness Demo"
        label.font = UIFont.etelka_34
        return label
    }()

    private let versionsButton: UIButton = {
        let button = UIButton()
        button.translatesAutoresizingMaskIntoConstraints = false
        button.layer.cornerRadius = 15
        button.backgroundColor = UIColor.resolutionBlue
        button.setTitle("settings".localized(), for: .normal)
        button.titleLabel?.font = UIFont.etelka_15
        return button
    }()

    private let checkLivenessButton: UIButton = {
        let button = UIButton()
        button.translatesAutoresizingMaskIntoConstraints = false
        button.layer.cornerRadius = 15
        button.backgroundColor = UIColor.resolutionBlue
        button.setTitle("check_liveness".localized(), for: .normal)
        button.titleLabel?.font = UIFont.etelka_15
        return button
    }()

    // MARK: - Lifecycle

    override func loadView() {
        super.loadView()
        view.backgroundColor = .white
    }

    override func viewDidLoad() {
        configureLayout()
    }

    // MARK: - Properties

    private var configuration: LCLunaConfiguration?
    private var livenessAPI: LunaCore.LCOnlineLivenessProtocol?
}

// MARK: - LCBestShotDelegate

extension LivenessViewController: LMCameraDelegate {

    func bestShot(_ bestShot: LCBestShot, _ videoFile: String?) {
        DispatchQueue.main.async { [weak self] in
            guard let self else { return }
            self.dismiss(animated: true) { [weak self] in
                guard let self else { return }
                print("[LIVENESS DEMO] Best shot obtained successfully. Path to videofile: \(videoFile ?? "Path is empty")")
                self.presentMessage(title: "successful_liveness".localized(),
                                    message: nil,
                                    action: nil)
            }
        }
    }

    func error(_ error: LunaCamera.LMCameraError, _ videoFile: String?) {
        DispatchQueue.main.async { [weak self] in
            guard let self else { return }
            self.dismiss(animated: true) { [weak self] in
                guard let self else { return }
                
                switch error {
                case .accessStatus(_):
                    let action = AlertAction(title: "action.open_app_settings".localized()) {
                        UIApplication.openAppSettings()
                    }
                    self.presentMessage(title: "error_title".localized(),
                                        message: "failed_liveness".localized(),
                                        action: action)
                case .canceled:
                    print("[LIVENESS DEMO] Camera is closed by user event")
                    return
                default:
                    print("[LIVENESS DEMO] LMCamera has received error. Error description: \(error.localizedDescription)")
                    self.presentMessage(title: "error_title".localized(),
                                        message: "failed_liveness".localized(),
                                        action: nil)
                }
            }
        }
    }
}

// MARK: - Private

private extension LivenessViewController {

    func configureLayout() {
        view.addSubview(logo)
        view.addSubview(appName)
        view.addSubview(checkLivenessButton)
        view.addSubview(versionsButton)
        view.addSubview(introductionView)

        /// Introduction View
        NSLayoutConstraint.activate([
            introductionView.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor),
            introductionView.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor),
            introductionView.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor),
            introductionView.bottomAnchor.constraint(equalTo: view.safeAreaLayoutGuide.bottomAnchor)
        ])

        /// Liveness View
        NSLayoutConstraint.activate([
            logo.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor, constant: 90),
            logo.centerXAnchor.constraint(equalTo: view.centerXAnchor),
            logo.heightAnchor.constraint(equalToConstant: 70),
            logo.widthAnchor.constraint(equalToConstant: 250),

            appName.topAnchor.constraint(equalTo: logo.bottomAnchor, constant: 50),
            appName.centerXAnchor.constraint(equalTo: view.centerXAnchor),
            appName.heightAnchor.constraint(equalToConstant: 70),

            checkLivenessButton.bottomAnchor.constraint(equalTo: view.safeAreaLayoutGuide.bottomAnchor, constant: -75),
            checkLivenessButton.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: 25),
            checkLivenessButton.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -25),
            checkLivenessButton.heightAnchor.constraint(equalToConstant: 55),

            versionsButton.bottomAnchor.constraint(equalTo: checkLivenessButton.topAnchor, constant: -25),
            versionsButton.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: 25),
            versionsButton.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -25),
            versionsButton.heightAnchor.constraint(equalToConstant: 55)
        ])
    }

    func addActions() {
        checkLivenessButton.addTarget(self, action: #selector(checkLiveness), for: .touchUpInside)
        versionsButton.addTarget(self, action: #selector(openVersions), for: .touchUpInside)
    }

    func change(state: State) {
        switch state {
        case .intro:
            logo.isHidden = true
            appName.isHidden = true
            checkLivenessButton.isHidden = true
            versionsButton.isHidden = true
            introductionView.isHidden = false
        case .liveness:
            introductionView.isHidden = true
            logo.isHidden = false
            appName.isHidden = false
            checkLivenessButton.isHidden = false
            versionsButton.isHidden = false
        }
    }

    func configureLuna() {
        configuration = LCLunaConfiguration.defaultConfig()
        configuration?.bestShotConfiguration.livenessType = .byPhoto
        configuration?.bestShotConfiguration.estimationThreshold.ags = 0.5
        configuration?.bestShotConfiguration.numberOfBestShots = 1
        configuration?.bestShotConfiguration.livenessQuality = 0
        configuration?.bestShotConfiguration.bestShotInterval = 0.5
        configuration?.locationEnabled = false
        configuration?.interactionEnabled = false
        configuration?.usePrebuiltDescriptors = true
        configuration?.saveOnlyFaceVideo = true
#if SAVE_BESTSHOT
        configuration?.uploadImagesForLiveness = true
#endif
        configuration?.lunaAccountID = "12ed7399-f779-479c-8258-bbc45e6017af"
        configuration?.lunaServerURL = URL(string: "https://luna-api-aws.visionlabs.ru/6")
        guard let configuration else { return }
        livenessAPI = LunaWeb.LivenessAPIv6(configuration: configuration)
    }

    @objc private func checkLiveness() {
        guard let livenessAPI,
              let configuration else {
            presentMessage(title: "failded_liveness".localized(),
                           message: "message.out_of_resources".localized(),
                           action: nil)
            return
        }

        let controller = LMCameraBuilder.viewController(delegate: self,
                                                        configuration: configuration,
                                                        livenessAPI: livenessAPI,
                                                        recordVideo: true)
        controller.dismissHandler = { [weak self] in
            guard let self = self else {
                return
            }
            
            self.closeCameraScreen()
        }
        present(controller, animated: true)
    }

    @objc private func openVersions() {
        guard let configuration else { return }
        let viewController = LEAboutVC(lunaConfig: configuration)
        present(viewController, animated: true)
    }

    private func closeCameraScreen() {
        dismiss(animated: true) {
            print("[LIVENESS DEMO] Camera is closed by user event")
        }
    }

    private func presentMessage(title: String, message: String?, action: AlertAction?) {
        DispatchQueue.main.async { [weak self] in
            guard let self else { return }
            let alert = UIAlertController(title: title, message: message, preferredStyle: .alert)
            switch action {
            case .none:
                alert.addAction(.init(title: "Ok", style: .default, handler: nil))
            case .some(let action):
                let uiAction = UIAlertAction(title: action.title, style: .default) { (_) in
                    action.handler?()
                }
                alert.addAction(uiAction)
            }
            self.present(alert, animated: true, completion: nil)
        }
    }
}
