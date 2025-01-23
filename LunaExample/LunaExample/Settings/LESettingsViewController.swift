//
//  LESettingsViewController.swift
//  LunaExample
//
//  Created by IVAN CHIRKOV on 27.10.2020.
//

import UIKit
import LunaCore
import LunaCamera
import LunaWeb

enum ESettingsItem: Int, CaseIterable {
    case SETTING_ITEM_MULTIPART_BESTSHOTS
    case SETTING_ITEM_GLASSES_CHECK
    case SETTING_ITEM_AGGREGATIONS_FOR_SUNGLASSES
    case SETTING_ITEM_OCR
    case SETTING_ITEM_INTERACTIONS
    case SETTING_ITEM_SAVE_ONLY_FACE_VIDEO
    case SETTING_ITEM_VIDEO_RECORD_LENGTH
    case SETTING_ITEM_TRACK_FACE_IDENTITY
    case SETTING_ITEM_OCCLUDED_FACE
    case SETTING_ITEM_OCCLUDED_MOUTH
    case SETTING_ITEM_ADVANCED_SUNGLASSES
    case SETTING_ITEM_EYE_INJURY
    case SETTING_ITEM_START_DELAY
    case SETTING_ITEM_FACE_TIME_DURATION
    case SETTING_ITEM_COMPRESSION_QUALITY
    case SETTING_ITEM_DOC_COMPARE
    case SETTING_ITEM_PRIMARY_FACE_MATHING
}

class LESettingsViewController: UIViewController, UITableViewDelegate, UITableViewDataSource, LMCameraDelegate {
    
    private enum Sections: Int, CaseIterable {
        case platformSection = 0
        case fsdkLicenseSection = 1
        case bestShotSection = 2
        case interactionsSection = 3
        case lunaConfigSection = 4
        case deletionSection = 5
        case versionSection = 6
        case buttonsSection = 7
    }
    
    private let CloseButtonSize: CGFloat = 44
    private let SideOffset: CGFloat = 10
    private let ButtonsSpace: CGFloat = 10
    private let LCRoundButtonHeight: CGFloat = 44

    private lazy var lunaAPI: LunaWeb.APIv6 = {
        APIv6(lunaAccountID: configuration.lunaAccountID,
              lunaServerURL: configuration.lunaPlatformURL) { [weak self] _ in
            guard let platformToken = self?.configuration.platformToken else { return [:] }
            return [APIv6Constants.Headers.authorization.rawValue: platformToken]
        }
    }()

    private var configuration = LunaCore.LCLunaConfiguration()

    private let tableView = UITableView(frame: .zero, style: .grouped)

    private var currentMinimalTrackLength: UInt?
    private var currentNumberOfBestShots: UInt?
    private var currentInteractionEnabled: Bool?

    override func loadView() {
        super.loadView()
        
        createLayout()
    }
    
    //  MARK: - LMCameraDelegate -
    
    func bestShot(_ bestShot: LCBestShot, _ videoFile: String?) {
        guard let jpegData = bestShot.getUIImageWarped(true).jpegData(compressionQuality: CGFloat(configuration.compressionQuality)) else {
            return
        }
        let bestShotData = BestShotData.image(jpegData)
        let query = EventQuery(bestShotsData: [bestShotData], imageType: .faceWarpedImage)

        lunaAPI.events.generateEvents(handlerID: configuration.identifyHandlerID, query: query) { [weak self] result in
            switch result {
            case .success(let response):
                self?.deleteFaceID(self?.parseFaceID(response))
            case .failure(_):
                DispatchQueue.main.async {
                    self?.dismiss(animated: true) {
                        self?.presentMessage("settings.error_while_search".localized())
                    }
                }
            }
        }
    }

    func multipartBestShots(_ bestShots: [LCBestShot], _ videoFile: String?) {}

    func error(_ error: LMCameraError, _ videoFile: String?) {
        DispatchQueue.main.async { [weak self] in
            self?.dismiss(animated: true) {
                self?.presentMessage("settings.error_bestshot".localized())
            }
        }
    }

    //  MARK: - UITableViewDelegate -
    
    func numberOfSections(in tableView: UITableView) -> Int {
        return Sections.allCases.count
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        var rowsNumber = 0
        
        switch section {
        case Sections.lunaConfigSection.rawValue:
            rowsNumber = ESettingsItem.allCases.count;
        default:
            rowsNumber = 1;
        }
        
        return rowsNumber
    }
    
    func tableView(_ tableView: UITableView, viewForFooterInSection section: Int) -> UIView? {
        return nil
    }

    func tableView(_ tableView: UITableView, viewForHeaderInSection section: Int) -> UIView? {
        let containerView = UIView(frame: .zero)
        containerView.backgroundColor = .clear
        
        let headerLabel = UILabel(frame: .zero)
        headerLabel.backgroundColor = .clear
        headerLabel.translatesAutoresizingMaskIntoConstraints = false
        headerLabel.font = Fonts.etelka_17
        headerLabel.textColor = .black
        containerView.addSubview(headerLabel)
        
        NSLayoutConstraint.activate([
            headerLabel.topAnchor.constraint(equalTo: containerView.topAnchor, constant: SideOffset),
            headerLabel.leadingAnchor.constraint(equalTo: containerView.leadingAnchor, constant: SideOffset),
            headerLabel.trailingAnchor.constraint(equalTo: containerView.trailingAnchor, constant: -SideOffset),
            headerLabel.bottomAnchor.constraint(equalTo: containerView.bottomAnchor, constant: -SideOffset),
        ])
        
        switch section {
        case Sections.deletionSection.rawValue:
            headerLabel.text = "settings.delete_registration".localized()
            headerLabel.textColor = .red
        case Sections.versionSection.rawValue:
            headerLabel.text = "settings.info_about_versions".localized()
        default:
            return nil
        }
        
        return containerView
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        var newCell = UITableViewCell(frame: .zero)
        
        switch indexPath.section {
        case Sections.platformSection.rawValue:
            let newSettingsCell = LELabelledCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.platform_config".localized(), .lunaBlue())
            newCell = newSettingsCell
        case Sections.fsdkLicenseSection.rawValue:
            let newSettingsCell = LELabelledCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.license_config".localized(), .lunaBlue())
            newCell = newSettingsCell
        case Sections.bestShotSection.rawValue:
            let newSettingsCell = LELabelledCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.bestshot_config".localized(), .lunaBlue())
            newCell = newSettingsCell
        case Sections.interactionsSection.rawValue:
            let newSettingsCell = LELabelledCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.interactions_config".localized(), .lunaBlue())
            newCell = newSettingsCell
        case Sections.lunaConfigSection.rawValue:
            newCell = createFeatureToggleSectionCell(indexPath.row)
        case Sections.deletionSection.rawValue:
            let newSettingsCell = LELabelledCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.delete_title".localized().uppercased(), .red)
            newCell = newSettingsCell
        case Sections.versionSection.rawValue:
            let newSettingsCell = LELabelledCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.versions".localized(), .lunaBlue())
            newCell = newSettingsCell
        case Sections.buttonsSection.rawValue:
            newCell = createButtonsSectionCell()
        default:
            break
        }
        
        return newCell
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        switch indexPath.section {
        case Sections.platformSection.rawValue:
            navigationController?.pushViewController(LEPlatfornSettingsVC(), animated: true)
        case Sections.fsdkLicenseSection.rawValue:
            navigationController?.pushViewController(LEFsdkLicenseSettingsVC(), animated: true)
        case Sections.bestShotSection.rawValue:
            navigationController?.pushViewController(LEBestshotSettingsVC(), animated: true)
        case Sections.interactionsSection.rawValue:
            navigationController?.pushViewController(LEInteractionsSettingsVC(), animated: true)
        case Sections.lunaConfigSection.rawValue:
            processFeatureToggleSectionTap(indexPath.row)
        case Sections.deletionSection.rawValue:
            startDeletion()
        case Sections.versionSection.rawValue:
            navigationController?.pushViewController(LEAboutVC(lunaConfig: configuration), animated: true)
        default:
            break
        }
    }
    
    func tableView(_ tableView: UITableView, heightForRowAt indexPath: IndexPath) -> CGFloat {
        switch indexPath.section {
        case Sections.bestShotSection.rawValue,
                Sections.interactionsSection.rawValue,
                Sections.deletionSection.rawValue,
                Sections.versionSection.rawValue:
            return 60
        default:
            return UITableView.automaticDimension
        }
    }
    
    //  MARK: - Routine -
    
    private func presentMessage(_ message: String) {
        let alert = UIAlertController(title: message, message: nil, preferredStyle: .alert)
        alert.addAction(.init(title: "OK", style: .default, handler: nil))
        present(alert, animated: true, completion: nil)
    }

    private func parseFaceID(_ response: LunaWeb.APIv6.EventsResponse) -> String? {
        guard let firstEvent = response.events.first else {
            return nil
        }
        
        guard let firstMatch = firstEvent.matches?.first else {
            return nil
        }
        
        guard let firstCandidate = firstMatch.candidates.first else {
            return nil
        }
        
        return firstCandidate.face.id
    }
    
    private func deleteFaceID(_ faceID: String?) {
        guard let faceID = faceID else {
            DispatchQueue.main.async { [weak self] in
                self?.dismiss(animated: true) {
                    self?.presentMessage("settings.face_not_found".localized())
                }
            }
            return
        }
        
        lunaAPI.faces.deleteFace(faceID: faceID) { [weak self] result in
            DispatchQueue.main.async {
                self?.dismiss(animated: true) {
                    self?.presentMessage("settings.deletion_successfull".localized())
                }
            }
        }
    }

    private func processFeatureToggleSectionTap(_ cellIndex: Int) {
        let settingsItem = ESettingsItem(rawValue: cellIndex)
        switch settingsItem {
        case .SETTING_ITEM_DOC_COMPARE:
            showDurationPicker("settings.doc_match".localized(), 1.0, 10, configuration.documentVerificationMatch) { newMatch in
                self.configuration.documentVerificationMatch = newMatch
                self.tableView.reloadData()
            }
        case .SETTING_ITEM_COMPRESSION_QUALITY:
            showDurationPicker("settings.compression_quality".localized(), 1.0, 10, configuration.compressionQuality) { newQuality in
                self.configuration.compressionQuality = newQuality
                self.tableView.reloadData()
            }
        case .SETTING_ITEM_FACE_TIME_DURATION:
            showDurationPicker("settings.face_time_duration".localized(), 3600, 1, configuration.faceTime) { newTime in
                self.configuration.faceTime = newTime
                self.tableView.reloadData()
            }
        case .SETTING_ITEM_VIDEO_RECORD_LENGTH:
            showDurationPicker("settings.video_record_length".localized(), 3600, 1, configuration.faceTime) { newTime in
                self.configuration.videoRecordLength = newTime
                self.tableView.reloadData()
            }
        case .SETTING_ITEM_START_DELAY:
            showDurationPicker("settings.start_delay".localized(), 3600, 1, configuration.startDelay) { newTime in
                self.configuration.startDelay = newTime
                self.tableView.reloadData()
            }
        case .SETTING_ITEM_PRIMARY_FACE_MATHING:
            showDurationPicker("settings.primary_face_matching".localized(), 1.0, 10, configuration.primaryFaceMatching) { newDegree in
                self.configuration.primaryFaceMatching = newDegree
                self.tableView.reloadData()
            }
        default:
            break
        }
    }
    
    private func showDurationPicker(_ title: String, _ limit: CGFloat, _ limitPower: CGFloat, _ initialValue: CGFloat, _ changeHandler: @escaping DurationChangedHandler) {
        let pickerVC = LEDurationPickerVC(title: title, limit: limit, limitPower: limitPower, initialValue: initialValue)
        pickerVC.periodChangedHandler = changeHandler
        view.addSubview(pickerVC.view)
        addChild(pickerVC)
    }
    
    private func createLayout() {
        view.backgroundColor = .white
        navigationItem.backButtonTitle = ""
        
        let closeButton = UIButton(frame: .zero)
        closeButton.translatesAutoresizingMaskIntoConstraints = false
        closeButton.setImage(UIImage(named: "close", in: Bundle(for: LESettingsViewController.self), compatibleWith: nil), for: .normal)
        closeButton.addTarget(self, action: #selector(closeViewController), for: .touchUpInside)
        closeButton.tintColor = UIColor(named: "LunaBlue")
        view.addSubview(closeButton)
        
        tableView.translatesAutoresizingMaskIntoConstraints = false
        tableView.backgroundColor = .clear
        tableView.showsVerticalScrollIndicator = false
        tableView.showsHorizontalScrollIndicator = false
        tableView.dataSource = self
        tableView.delegate = self
        tableView.separatorStyle = .none
        view.addSubview(tableView)
        
        NSLayoutConstraint.activate([
            closeButton.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor, constant: SideOffset),
            closeButton.trailingAnchor.constraint(equalTo: view.trailingAnchor, constant: -SideOffset),
            closeButton.widthAnchor.constraint(equalToConstant: CloseButtonSize),
            closeButton.heightAnchor.constraint(equalToConstant: CloseButtonSize),
            
            tableView.topAnchor.constraint(equalTo: closeButton.bottomAnchor, constant: SideOffset),
            tableView.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor),
            tableView.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor),
            tableView.bottomAnchor.constraint(equalTo: view.safeAreaLayoutGuide.bottomAnchor)
        ])
    }
    
    private func startDeletion() {
        let livenessAPI = LunaWeb.LivenessAPIv6(configuration: configuration) { [weak self] _ in
            guard let platformToken = self?.configuration.platformToken else { return [:] }
            return [APIv6Constants.Headers.authorization.rawValue: platformToken]
        }
        let controller = LMCameraBuilder.viewController(delegate: self,
                                                        configuration: LCLunaConfiguration(),
                                                        livenessAPI: livenessAPI,
                                                        canRecordVideo: false)
        controller.modalPresentationStyle = .fullScreen
        controller.dismissHandler = { [weak self] in
            guard let self = self else {
                return
            }
            
            self.dismiss(animated: true)
        }
        
        present(controller, animated: true)
    }
    
    private func createFeatureToggleSectionCell(_ cellIndex: Int) -> UITableViewCell {
        var newCell = UITableViewCell(frame: .zero)
        
        let settingsItem = ESettingsItem(rawValue: cellIndex)
        switch settingsItem {
        case .SETTING_ITEM_MULTIPART_BESTSHOTS:
            let newSettingsCell = LELabelledToggleCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.toggleStatusHandler = { [weak self] toggleStatus in
                self?.configuration.multipartBestShotsEnabled = toggleStatus
            }
            newSettingsCell.configureCell(configuration.multipartBestShotsEnabled, "settings.multipartBestShots_enabled".localized())
            newCell = newSettingsCell
        case .SETTING_ITEM_VIDEO_RECORD_LENGTH:
            let newSettingsCell = LEFloatCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.video_record_length".localized(), configuration.videoRecordLength)
            newCell = newSettingsCell
            break;
        case .SETTING_ITEM_GLASSES_CHECK:
            let newSettingsCell = LELabelledToggleCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.toggleStatusHandler = { [weak self] toggleStatus in
                self?.configuration.glassesCheckEnabled = toggleStatus
            }
            newSettingsCell.configureCell(configuration.glassesCheckEnabled, "settings.glassescheck_enabled".localized())
            newCell = newSettingsCell
        case .SETTING_ITEM_AGGREGATIONS_FOR_SUNGLASSES:
            let newSettingsCell = LELabelledToggleCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.toggleStatusHandler = { [weak self] toggleStatus in
                self?.configuration.aggregationEnabled = toggleStatus
            }
            newSettingsCell.configureCell(configuration.aggregationEnabled, "settings.aggregation_enabled".localized())
            newCell = newSettingsCell
        case .SETTING_ITEM_OCR:
            let newSettingsCell = LELabelledToggleCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.toggleStatusHandler = { [weak self] toggleStatus in
                self?.configuration.ocrEnabled = toggleStatus
            }
            newSettingsCell.configureCell(configuration.ocrEnabled, "settings.ocr_enabled".localized())
            newCell = newSettingsCell
        case .SETTING_ITEM_INTERACTIONS:
            let newSettingsCell = LELabelledToggleCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.toggleStatusHandler = { [weak self] toggleStatus in
                self?.configuration.interactionEnabled = toggleStatus
            }
            newSettingsCell.configureCell(configuration.interactionEnabled, "settings.interaction_enabled".localized())
            newCell = newSettingsCell
        case .SETTING_ITEM_SAVE_ONLY_FACE_VIDEO:
            let newSettingsCell = LELabelledToggleCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.toggleStatusHandler = { [weak self] toggleStatus in
                self?.configuration.saveOnlyFaceVideo = toggleStatus
            }
            newSettingsCell.configureCell(configuration.saveOnlyFaceVideo, "settings.onlyface_enabled".localized())
            newCell = newSettingsCell
        case .SETTING_ITEM_TRACK_FACE_IDENTITY:
            let newSettingsCell = LELabelledToggleCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.toggleStatusHandler = { [weak self] toggleStatus in
                self?.configuration.trackFaceIdentity = toggleStatus
            }
            newSettingsCell.configureCell(configuration.trackFaceIdentity, "settings.trackfaceidentity_enabled".localized())
            newCell = newSettingsCell
        case .SETTING_ITEM_OCCLUDED_FACE:
            let newSettingsCell = LELabelledToggleCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.toggleStatusHandler = { [weak self] toggleStatus in
                self?.configuration.occludeCheck = toggleStatus
            }
            newSettingsCell.configureCell(configuration.occludeCheck, "settings.occludedcheck_enabled".localized())
            newCell = newSettingsCell
        case .SETTING_ITEM_OCCLUDED_MOUTH:
            let newSettingsCell = LELabelledToggleCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.toggleStatusHandler = { [weak self] toggleStatus in
                self?.configuration.mouthCheck = toggleStatus
            }
            newSettingsCell.configureCell(configuration.mouthCheck, "settings.mouthcheck_enabled".localized())
            newCell = newSettingsCell
        case .SETTING_ITEM_ADVANCED_SUNGLASSES:
            let newSettingsCell = LELabelledToggleCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.toggleStatusHandler = { [weak self] advancedSunglasses in
                self?.configuration.advancedSunglasses = advancedSunglasses
            }
            newSettingsCell.configureCell(configuration.advancedSunglasses, "settings.advanced_sunglasses".localized())
            newCell = newSettingsCell
        case .SETTING_ITEM_EYE_INJURY:
            let newSettingsCell = LELabelledToggleCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.toggleStatusHandler = { [weak self] eyeInjury in
                self?.configuration.eyeInjury = eyeInjury
            }
            newSettingsCell.configureCell(configuration.eyeInjury, "settings.eye_injury".localized())
            newCell = newSettingsCell
        case .SETTING_ITEM_START_DELAY:
            let newSettingsCell = LEFloatCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.start_delay".localized(), configuration.startDelay)
            newCell = newSettingsCell
            break;
        case .SETTING_ITEM_FACE_TIME_DURATION:
            let newSettingsCell = LEFloatCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.face_time_duration".localized(), configuration.faceTime)
            newCell = newSettingsCell
            break;
        case .SETTING_ITEM_COMPRESSION_QUALITY:
            let newSettingsCell = LEFloatCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.compression_quality".localized(), configuration.compressionQuality)
            newCell = newSettingsCell
            break;
        case .SETTING_ITEM_DOC_COMPARE:
            let newSettingsCell = LEFloatCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.doc_match".localized(), configuration.documentVerificationMatch)
            newCell = newSettingsCell
            break;
        case .SETTING_ITEM_PRIMARY_FACE_MATHING:
            let newSettingsCell = LEFloatCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.primary_face_matching".localized(), configuration.primaryFaceMatching)
            newCell = newSettingsCell
        default:
            break
        }
        
        return newCell
    }

    private func createButtonsSectionCell() -> UITableViewCell {
        let newCell = UITableViewCell(frame: .zero)

        let resetButton = LCRoundButton(type: .custom)
        resetButton.translatesAutoresizingMaskIntoConstraints = false
        resetButton.setTitle("settings.reset_to_defaults".localized(), for: .normal)
        resetButton.addTarget(self, action: #selector(resetToDefaultButtonTapped), for: .touchUpInside)
        resetButton.isEnabled = true

        let applyFromPlistButton = LCRoundButton(type: .custom)
        applyFromPlistButton.translatesAutoresizingMaskIntoConstraints = false
        applyFromPlistButton.setTitle("settings.apply_from_plist".localized(), for: .normal)
        applyFromPlistButton.addTarget(self, action: #selector(applyFromPlistButtonTapped), for: .touchUpInside)
        applyFromPlistButton.isEnabled = true

        let checkPhotoButton = LCRoundButton(type: .custom)
        checkPhotoButton.translatesAutoresizingMaskIntoConstraints = false
        checkPhotoButton.setTitle("settings.check_photo".localized(), for: .normal)
        checkPhotoButton.addTarget(self, action: #selector(checkPhotoButtonTapped), for: .touchUpInside)
        checkPhotoButton.isEnabled = true

        newCell.contentView.addSubview(resetButton)
        newCell.contentView.addSubview(applyFromPlistButton)
        newCell.contentView.addSubview(checkPhotoButton)

        NSLayoutConstraint.activate([
            resetButton.topAnchor.constraint(equalTo: newCell.contentView.topAnchor),
            resetButton.leadingAnchor.constraint(equalTo: newCell.contentView.leadingAnchor, constant: SideOffset),
            resetButton.trailingAnchor.constraint(equalTo: newCell.contentView.trailingAnchor, constant: -SideOffset),
            resetButton.heightAnchor.constraint(equalToConstant: LCRoundButtonHeight),

            applyFromPlistButton.topAnchor.constraint(equalTo: resetButton.bottomAnchor, constant: ButtonsSpace),
            applyFromPlistButton.leadingAnchor.constraint(equalTo: newCell.contentView.leadingAnchor, constant: SideOffset),
            applyFromPlistButton.trailingAnchor.constraint(equalTo: newCell.contentView.trailingAnchor, constant: -SideOffset),
            applyFromPlistButton.heightAnchor.constraint(equalToConstant: LCRoundButtonHeight),

            checkPhotoButton.topAnchor.constraint(equalTo: applyFromPlistButton.bottomAnchor, constant: ButtonsSpace),
            checkPhotoButton.leadingAnchor.constraint(equalTo: newCell.contentView.leadingAnchor, constant: SideOffset),
            checkPhotoButton.trailingAnchor.constraint(equalTo: newCell.contentView.trailingAnchor, constant: -SideOffset),
            checkPhotoButton.heightAnchor.constraint(equalToConstant: LCRoundButtonHeight),
            checkPhotoButton.bottomAnchor.constraint(equalTo: newCell.contentView.bottomAnchor),
        ])

        return newCell
    }

    //  MARK: - Handlers -
    
    @objc
    private func resetToDefaultButtonTapped() {
        configuration = LCLunaConfiguration.defaultConfig()
        configuration.licenseConfig.flushIntoUserDefaults()
        tableView.reloadData()
    }

    @objc
    private func applyFromPlistButtonTapped() {
        let completion: (URL) -> Void = { [weak self] plistFileURL in
            let plistName = plistFileURL.lastPathComponent
            self?.configuration = LCLunaConfiguration(plistFromDocuments: plistName)
            self?.tableView.reloadData()
        }

        let viewController = LEDocumentsFileListVC(pathExtension: "plist",
                                                   completionMode: .plistFiles(completion: completion))

        navigationController?.pushViewController(viewController, animated: true)
    }

    @objc
    private func checkPhotoButtonTapped() {
        currentNumberOfBestShots = configuration.bestShotConfiguration.numberOfBestShots
        currentInteractionEnabled = configuration.interactionEnabled

        configuration.bestShotConfiguration.numberOfBestShots = 1
        configuration.interactionEnabled = false

        let completion: (Result<LCBestShot, Error>) -> Void = { [weak self] result in
            guard
                let self = self,
                let currentMinimalTrackLength = self.currentMinimalTrackLength,
                let currentNumberOfBestShots = self.currentNumberOfBestShots,
                let currentInteractionEnabled = self.currentInteractionEnabled
            else { return }

            self.configuration.bestShotConfiguration.numberOfBestShots = currentNumberOfBestShots
            self.configuration.interactionEnabled = currentInteractionEnabled

            switch result {
            case .success(_):
                self.presentMessage("settings.success_bestshot".localized())
            case .failure(let error):
                self.presentModalError(error.what())
            }
        }

        let viewController = LEDocumentsFileListVC(pathExtension: nil,
                                                   configuration: configuration,
                                                   completionMode: .imageBestShot(completion))

        navigationController?.pushViewController(viewController, animated: true)
    }

    @objc
    private func closeViewController() {
        navigationController?.dismiss(animated: true)
    }
    
}
