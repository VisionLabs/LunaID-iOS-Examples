//
//  LEBestshotSettingsVC.swift
//  LunaExample
//
//  Created by IVAN CHIRKOV on 27.10.2020.
//

import UIKit
import LunaCore
import LunaCamera
import LunaWeb

enum EBestshotSettingsItem: Int {
    
    case BESTSHOTSETTING_ITEM_EYES_CHECK
    case BESTSHOTSETTING_ITEM_LIVENESS_TYPE
    case BESTSHOTSETTING_ITEM_BORDER_DISTANCE
    case BESTSHOTSETTING_ITEM_MIN_DET_SIZE
    case BESTSHOTSETTING_ITEM_NUMBER_OF_BESTSHOTS
    case BESTSHOTSETTING_ITEM_BESTSHOTS_INTERVAL
    case BESTSHOTSETTING_ITEM_LIVENESS_QUALITY

    case BESTSHOTSETTING_ITEM_COUNT
    
}

enum EThresholdSettingsItem: Int {
    
    case THRESHOLD_ITEM_YAW
    case THRESHOLD_ITEM_PITCH
    case THRESHOLD_ITEM_ROLL
    case THRESHOLD_ITEM_AGS

    case THRESHOLD_ITEM_COUNT
}

enum ELivenessType: String, CaseIterable {
    
    case disabled = "liveness_type_disabled"
    case byImage = "liveness_type_by_image"
    case offline = "liveness_type_offline"
    
}

class LEBestshotSettingsVC: UIViewController, UITableViewDelegate, UITableViewDataSource {
    
    private let SideOffset: CGFloat = 10
    private var config: LunaCore.LCLunaConfiguration
    private let tableView = UITableView(frame: .zero, style: .grouped)
    
    init(configuration: LunaCore.LCLunaConfiguration) {
        self.config = configuration
        super.init(nibName: nil, bundle: nil)
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    override func loadView() {
        super.loadView()
        
        createLayout()
    }
    
    public override func viewWillDisappear(_ animated: Bool) {
        navigationController?.navigationBar.isHidden = true
    }
    
    public override func viewWillAppear(_ animated: Bool) {
        navigationController?.navigationBar.isHidden = false
    }

    //  MARK: - UITableViewDelegate -
    
    func numberOfSections(in tableView: UITableView) -> Int {
        return 2
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        if (section == 1) {
            return 4;
        }
        
        return EBestshotSettingsItem.BESTSHOTSETTING_ITEM_COUNT.rawValue;
    }
    
    func tableView(_ tableView: UITableView, viewForFooterInSection section: Int) -> UIView? {
        return nil
    }
    
    func tableView(_ tableView: UITableView, viewForHeaderInSection section: Int) -> UIView? {
        if section != 1 {
            return nil
        }
        
        let containerView = UIView(frame: .zero)
        containerView.backgroundColor = .clear
        
        let headerLabel = UILabel(frame: .zero)
        headerLabel.backgroundColor = .clear
        headerLabel.translatesAutoresizingMaskIntoConstraints = false
        headerLabel.font = Fonts.etelka_17
        headerLabel.textColor = .black
        headerLabel.text = "settings.threshold_title".localized()
        containerView.addSubview(headerLabel)
        
        NSLayoutConstraint.activate([
            headerLabel.topAnchor.constraint(equalTo: containerView.topAnchor, constant: SideOffset),
            headerLabel.leadingAnchor.constraint(equalTo: containerView.leadingAnchor, constant: SideOffset),
            headerLabel.trailingAnchor.constraint(equalTo: containerView.trailingAnchor, constant: -SideOffset),
            headerLabel.bottomAnchor.constraint(equalTo: containerView.bottomAnchor, constant: -SideOffset),
        ])
        
        return containerView
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        if (indexPath.section == 1) {
            return createThresholdSectionCell(indexPath.row)
        }
        
        return createFeatureToggleSectionCell(indexPath.row)
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        if (indexPath.section == 1) {
            processThresholdSectionTap(indexPath.row)
            return
        }
        processFeatureToggleSectionTap(indexPath.row)
    }
    
    func tableView(_ tableView: UITableView, heightForRowAt indexPath: IndexPath) -> CGFloat {
        return 60
    }
    
    //  MARK: - Routine -
    
    private func createLayout() {
        view.backgroundColor = .white
        navigationItem.backButtonTitle = ""
        
        tableView.translatesAutoresizingMaskIntoConstraints = false
        tableView.backgroundColor = .clear
        tableView.showsVerticalScrollIndicator = false
        tableView.showsHorizontalScrollIndicator = false
        tableView.dataSource = self
        tableView.delegate = self
        tableView.separatorStyle = .none
        view.addSubview(tableView)
        
        NSLayoutConstraint.activate([
            tableView.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor),
            tableView.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor),
            tableView.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor),
            tableView.bottomAnchor.constraint(equalTo: view.safeAreaLayoutGuide.bottomAnchor)
        ])
    }
    
    private func processThresholdSectionTap(_ cellIndex: Int) {
        let settingsItem = EThresholdSettingsItem(rawValue: cellIndex)
        
        switch settingsItem {
        case .THRESHOLD_ITEM_YAW:
            showDurationPicker("settings.threshold_yaw".localized(), config.bestShotConfiguration.headYaw, 100, 1, { newValue in
                self.config.bestShotConfiguration.headYaw = newValue
                self.tableView.reloadData()
            })
        case .THRESHOLD_ITEM_ROLL:
            showDurationPicker("settings.threshold_roll".localized(), config.bestShotConfiguration.headRoll, 100, 1, { newValue in
                self.config.bestShotConfiguration.headRoll = newValue
                self.tableView.reloadData()
            })
        case .THRESHOLD_ITEM_PITCH:
            showDurationPicker("settings.threshold_pitch".localized(), config.bestShotConfiguration.headPitch, 100, 1, { newValue in
                self.config.bestShotConfiguration.headPitch = newValue
                self.tableView.reloadData()
            })
        case .THRESHOLD_ITEM_AGS:
            showDurationPicker("settings.threshold_ags".localized(), config.bestShotConfiguration.ags, 1, 10, { newValue in
                self.config.bestShotConfiguration.ags = newValue
                self.tableView.reloadData()
            })
        default:
            break
        }
    }
    
    private func processFeatureToggleSectionTap(_ cellIndex: Int) {
        let settingsItem = EBestshotSettingsItem(rawValue: cellIndex)
        switch settingsItem {
        case .BESTSHOTSETTING_ITEM_LIVENESS_TYPE:
            showEnumPicker("settings.liveness".localized(), allLiveness(), 0) { newValue in
                self.config.bestShotConfiguration.livenessType = self.convertNewValue(newValue)
                self.tableView.reloadData()
            }
        case .BESTSHOTSETTING_ITEM_BORDER_DISTANCE:
            showDurationPicker("settings.border_distance".localized(), CGFloat(config.bestShotConfiguration.borderDistance), 1000, 1, { newBorderDistance in
                self.config.bestShotConfiguration.borderDistance = NSInteger(newBorderDistance)
                self.tableView.reloadData()
            })
        case .BESTSHOTSETTING_ITEM_MIN_DET_SIZE:
            showDurationPicker("settings.min_det_size".localized(), CGFloat(config.bestShotConfiguration.minDetSize), 1000, 1, { newMinDetSize in
                self.config.bestShotConfiguration.minDetSize = NSInteger(newMinDetSize)
                self.tableView.reloadData()
            })
        case .BESTSHOTSETTING_ITEM_NUMBER_OF_BESTSHOTS:
            showDurationPicker("settings.number_of_bestshots".localized(), CGFloat(config.bestShotConfiguration.numberOfBestShots), 1000, 1, { newNumberOfBestShots in
                self.config.bestShotConfiguration.numberOfBestShots = UInt(newNumberOfBestShots)
                self.tableView.reloadData()
            })
        case .BESTSHOTSETTING_ITEM_BESTSHOTS_INTERVAL:
            showDurationPicker("settings.bestshots_interval".localized(), CGFloat(config.bestShotConfiguration.bestShotInterval), 1000, 10.0, { newBestShotInterval in
                self.config.bestShotConfiguration.bestShotInterval = newBestShotInterval
                self.tableView.reloadData()
            })
        case .BESTSHOTSETTING_ITEM_LIVENESS_QUALITY:
            showDurationPicker("settings.liveness_quality".localized(), CGFloat(config.bestShotConfiguration.livenessQuality), 1, 10.0, { newLivenessQuality in
                self.config.bestShotConfiguration.livenessQuality = newLivenessQuality
                self.tableView.reloadData()
            })
        default:
            break
        }
    }
    
    private func createThresholdSectionCell(_ cellIndex: Int) -> UITableViewCell {
        var newCell = UITableViewCell(frame: .zero)
        
        let settingsItem = EThresholdSettingsItem(rawValue: cellIndex)
        switch settingsItem {
        case .THRESHOLD_ITEM_YAW:
            let newSettingsCell = LEFloatCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.threshold_yaw".localized(), config.bestShotConfiguration.headYaw)
            newCell = newSettingsCell
        case .THRESHOLD_ITEM_ROLL:
            let newSettingsCell = LEFloatCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.threshold_roll".localized(), config.bestShotConfiguration.headRoll)
            newCell = newSettingsCell
        case .THRESHOLD_ITEM_PITCH:
            let newSettingsCell = LEFloatCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.threshold_pitch".localized(), config.bestShotConfiguration.headPitch)
            newCell = newSettingsCell
        case .THRESHOLD_ITEM_AGS:
            let newSettingsCell = LEFloatCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.threshold_ags".localized(), config.bestShotConfiguration.ags)
            newCell = newSettingsCell
        default:
            break
        }
        
        return newCell
    }
    
    private func createFeatureToggleSectionCell(_ cellIndex: Int) -> UITableViewCell {
        var newCell = UITableViewCell(frame: .zero)
        
        var settingsItem = EBestshotSettingsItem(rawValue: cellIndex)
        switch settingsItem {
        case .BESTSHOTSETTING_ITEM_EYES_CHECK:
            let newSettingsCell = LELabelledToggleCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.toggleStatusHandler = { [weak self] toggleStatus in
                self?.config.bestShotConfiguration.eyesCheck = toggleStatus
            }
            newSettingsCell.configureCell(config.bestShotConfiguration.eyesCheck, "settings.eyescheck_enabled".localized())
            newCell = newSettingsCell
        case .BESTSHOTSETTING_ITEM_LIVENESS_TYPE:
            let newSettingsCell = LEEnumCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("Liveness", allLiveness()[Int(config.bestShotConfiguration.livenessType.rawValue)].localized())
            newCell = newSettingsCell
        case .BESTSHOTSETTING_ITEM_BORDER_DISTANCE:
            let newSettingsCell = LEFloatCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.border_distance".localized(), CGFloat(config.bestShotConfiguration.borderDistance))
            newCell = newSettingsCell
        case .BESTSHOTSETTING_ITEM_MIN_DET_SIZE:
            let newSettingsCell = LEFloatCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.min_det_size".localized(), CGFloat(config.bestShotConfiguration.minDetSize))
            newCell = newSettingsCell
        case .BESTSHOTSETTING_ITEM_NUMBER_OF_BESTSHOTS:
            let newSettingsCell = LEFloatCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.number_of_bestshots".localized(), CGFloat(config.bestShotConfiguration.numberOfBestShots))
            newCell = newSettingsCell
        case .BESTSHOTSETTING_ITEM_BESTSHOTS_INTERVAL:
            let newSettingsCell = LEFloatCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.bestshots_interval".localized(), config.bestShotConfiguration.bestShotInterval)
            newCell = newSettingsCell
            break;
        case .BESTSHOTSETTING_ITEM_LIVENESS_QUALITY:
            let newSettingsCell = LEFloatCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.liveness_quality".localized(), config.bestShotConfiguration.livenessQuality)
            newCell = newSettingsCell
            break;
        default:
            break
        }
        
        return newCell
    }
    
    private func showDurationPicker(_ title: String, _ initialValue: CGFloat, _ limit: CGFloat, _ limitPower: CGFloat, _ changeHandler: @escaping DurationChangedHandler) {
        let pickerVC = LEDurationPickerVC(title: title, limit: limit, limitPower: limitPower, initialValue: initialValue)
        pickerVC.periodChangedHandler = changeHandler
        view.addSubview(pickerVC.view)
        addChild(pickerVC)
    }

    private func convertNewValue(_ newValue: String) -> LivenessType {
        let bestShotLivenessType = ELivenessType(rawValue: newValue)
        
        switch bestShotLivenessType {
        case .disabled:
            return LivenessType.none
        case .byImage:
            return LivenessType.byPhoto
        case .offline:
            return LivenessType.offline
        default:
            return LivenessType.none
        }
    }
    
    private func allLiveness() -> [String] {
        return ELivenessType.allCases.map { "\($0.rawValue)" }
    }
    
    private func showEnumPicker(_ title: String, _ enumList: [String], _ initialValueIndex: Int, _ enumChangedHandler: @escaping EnumChangedHandler) {
        let pickerVC = LEEnumPickerVC(title: title, enumList: enumList, initialValueIndex: initialValueIndex)
        pickerVC.enumChangedHandler = enumChangedHandler
        view.addSubview(pickerVC.view)
        addChild(pickerVC)
    }
    
}
