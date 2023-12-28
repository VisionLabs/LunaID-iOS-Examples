//
//  ViewController.swift
//  Liveness Demo
//
//  Created by IVAN CHIRKOV on 05.05.2021.
//

import UIKit
import LunaCore
import LunaCamera
import LunaWeb

class StartViewController: UIViewController {

    @IBOutlet private weak var bestShotsNumberLabel: UILabel!
    @IBOutlet private weak var bestShotsNumberSlider: UISlider!
    @IBOutlet private weak var timeIntervalLabel: UILabel!
    @IBOutlet private weak var timeIntervalSlider: UISlider!
    @IBOutlet private weak var livenessQualityLabel: UILabel!
    @IBOutlet private weak var livenessQualitySlider: UISlider!
    @IBOutlet private weak var agsLabel: UILabel!
    @IBOutlet private weak var agsSlider: UISlider!
    
    var livenessAPI: LunaCore.LCOnlineLivenessProtocol?
    
    private lazy var configuration: LCLunaConfiguration = {
        let configuration = LCLunaConfiguration.defaultConfig()
        configuration.bestShotConfiguration.livenessType = .byPhoto
        configuration.bestShotConfiguration.estimationThreshold.ags = 0.5
        configuration.bestShotConfiguration.numberOfBestShots = 1
        configuration.bestShotConfiguration.livenessQuality = 0
        configuration.bestShotConfiguration.bestShotInterval = 0.5
        
        configuration.compressionQuality = 0.8
        configuration.imageType = .JPEG
        
        configuration.locationEnabled = false
        configuration.interactionEnabled = false
        configuration.usePrebuiltDescriptors = true
#if SAVE_BESTSHOT
        configuration.uploadImagesForLiveness = true
#endif
        return configuration
    }()

    override func viewDidLoad() {
        super.viewDidLoad()
        bestShotsNumberSlider.value = Float(configuration.bestShotConfiguration.numberOfBestShots)
        bestShotsNumberLabel.text = "Number of best shots: \(configuration.bestShotConfiguration.numberOfBestShots)"
        timeIntervalSlider.value = Float(configuration.bestShotConfiguration.bestShotInterval)
        timeIntervalLabel.text = "Time interval: \(configuration.bestShotConfiguration.bestShotInterval)"
        livenessQualitySlider.value = Float(configuration.bestShotConfiguration.livenessQuality)
        livenessQualityLabel.text = "Liveness quality: \(configuration.bestShotConfiguration.livenessQuality)"
        agsSlider.value = Float(configuration.bestShotConfiguration.estimationThreshold.ags)
        agsLabel.text = "AGS: \(configuration.bestShotConfiguration.estimationThreshold.ags)"
        
        livenessAPI = LunaWeb.LivenessAPIv6(configuration: configuration)
    }

    @IBAction private func checkLiveness(_ sender: UIButton) {
        guard let livenessAPI = livenessAPI else { return }
        
        let controller = LMCameraBuilder.viewController(delegate: self,
                                                          configuration: configuration,
                                                          livenessAPI: livenessAPI)
        present(controller, animated: true)
    }

    @IBAction private func showVersionsButtonTapped(_ sender: UIButton) {
        let viewController = LEAboutVC()
        present(viewController, animated: true)
    }

    private func presentMessage(_ message: String) {
        let alert = UIAlertController(title: message, message: nil, preferredStyle: .alert)
        alert.addAction(.init(title: "OK", style: .default, handler: nil))
        present(alert, animated: true, completion: nil)
    }

    private func sendImageToStore(_ image: UIImage) {

    }
    
    @IBAction private func sliderValueChanged(_ sender: UISlider) {
        switch true {
        case sender === bestShotsNumberSlider:
            let value = UInt(sender.value)
            bestShotsNumberLabel.text = "Number of best shots: \(value)"
            configuration.bestShotConfiguration.numberOfBestShots = value
        case sender === timeIntervalSlider:
            timeIntervalLabel.text = "Time interval: \(sender.value)"
            configuration.bestShotConfiguration.bestShotInterval = Double(sender.value)
        case sender === livenessQualitySlider:
            livenessQualityLabel.text = "Liveness quality: \(sender.value)"
            configuration.bestShotConfiguration.livenessQuality = CGFloat(sender.value)
        case sender === agsSlider:
            agsLabel.text = "AGS: \(sender.value)"
            configuration.bestShotConfiguration.estimationThreshold.ags = CGFloat(sender.value)
        default: break
        }
    }

}

extension StartViewController: LMCameraDelegate {
        
    func bestShot(_ bestShot: LCBestShot) {
        DispatchQueue.main.async {
            self.presentMessage(NSLocalizedString("common.liveness_check_passed", comment: ""))
        }
    }

    func error(error: LMCameraError) {
        DispatchQueue.main.async {
            if case LMCameraError.accessStatus = error {
                let action = AlertAction(title: "common.open_app_settings".localized()) {
                    UIApplication.openAppSettings()
                }
                self.presentError(error.what(), actions: [action])
            } else {
                switch error {
                case .canceled:
                    self.presentError(error.what(), needTitle: false)
                    
                case .error(let innerError):
                    guard let networkError = innerError as? LunaWeb.NetworkingError else {
                        self.presentError(error.what())
                        return
                    }
                    
                    switch networkError {
                    case .livenessPredictionFakeError:
                        self.presentError(error.what(), needTitle: false)

                    default:
                        self.presentError(error.what())
                    }
                    
                default:
                    self.presentError(error.what())
                }
            }
        }
    }

}
