//
//  LELicenseSettingsVC.swift
//  LunaExample
//
//  Created by Геращенко Андрей on 03.09.2024.
//

import UIKit
import LunaCore
import LunaCamera
import LunaWeb

class LELicenseSettingsVC: UIViewController, UITableViewDelegate, UITableViewDataSource {
    
    private enum LicenseSettings: Int {
        case connectionTimeOut = 0
        case serverRetriesCount = 1
        case EID = 2
        case productID = 3
    }
    
    private enum SettingsTitles: String {
        case connectionTimeout = "settings.license.connectionTimeOut_config"
        case serverRetriesCount = "settings.license.serverRetriesCount_config"
        case EID = "settings.license.EID_config"
        case productID = "settings.license.productID_config"
    }
    
    private var licenseConfig = LunaCore.LCLicenseConfig.userDefaults()
    private let tableView = UITableView(frame: .zero, style: .grouped)
    
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
    
    override func willMove(toParent parent: UIViewController?) {
        super.willMove(toParent: parent)
        if parent == nil {
            licenseConfig.save()
            LCLunaConfiguration.resetLicenseCache()
        }
    }

    //  MARK: - UITableViewDelegate -
    
    func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return 4;
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        var newCell: UITableViewCell?
        
        switch indexPath.row {
        case LicenseSettings.connectionTimeOut.rawValue:
            newCell = Self.makeFloatInputCell(title: SettingsTitles.connectionTimeout.rawValue,
                                              value: licenseConfig.connectionTimeout)
        case LicenseSettings.serverRetriesCount.rawValue:
            newCell = Self.makeFloatInputCell(title: SettingsTitles.serverRetriesCount.rawValue,
                                              value: Double(licenseConfig.serverRetriesCount))
        case LicenseSettings.EID.rawValue:
            newCell = Self.makeStringInputCell(title: SettingsTitles.EID.rawValue,
                                               value: licenseConfig.eid)
        case LicenseSettings.productID.rawValue:
            newCell = Self.makeStringInputCell(title: SettingsTitles.productID.rawValue,
                                               value: licenseConfig.productID)
        default:
            break
        }
        
        return newCell ?? UITableViewCell(frame: .zero)
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        switch indexPath.row {
        case LicenseSettings.connectionTimeOut.rawValue:
            showDurationPicker(SettingsTitles.connectionTimeout.rawValue.localized(),
                               120.0,
                               1.0,
                               CGFloat(licenseConfig.connectionTimeout)) { [weak self] newValue in
                self?.licenseConfig.connectionTimeout = newValue
                self?.tableView.reloadData()
            }
            case LicenseSettings.serverRetriesCount.rawValue:
            showDurationPicker(SettingsTitles.serverRetriesCount.rawValue.localized(),
                               50.0,
                               1.0,
                               CGFloat(licenseConfig.serverRetriesCount)) { [weak self] newValue in
                self?.licenseConfig.serverRetriesCount = Int(newValue)
                self?.tableView.reloadData()
            }
        case LicenseSettings.EID.rawValue:
            let inputVC = LEInputVC(initialText: licenseConfig.eid)
            inputVC.valueChangedHandler = { [weak self] text in
                self?.licenseConfig.eid = text ?? ""
                self?.tableView.reloadData()
            }
            self.navigationController?.pushViewController(inputVC, animated: true)
        case LicenseSettings.productID.rawValue:
            let inputVC = LEInputVC(initialText: licenseConfig.productID)
            inputVC.valueChangedHandler = { [weak self] text in
                self?.licenseConfig.productID = text ?? ""
                self?.tableView.reloadData()
            }
            self.navigationController?.pushViewController(inputVC, animated: true)
        default:
            break
        }
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
    
    private func showDurationPicker(_ title: String, 
                                    _ limit: CGFloat,
                                    _ limitPower: CGFloat,
                                    _ initialValue: CGFloat,
                                    _ changeHandler: @escaping DurationChangedHandler) {
        let pickerVC = LEDurationPickerVC(title: title,
                                          limit: limit,
                                          limitPower: limitPower,
                                          initialValue: initialValue)
        pickerVC.periodChangedHandler = changeHandler
        view.addSubview(pickerVC.view)
        addChild(pickerVC)
    }
    
    private static func makeFloatInputCell(title: String, value: Double) -> LEFloatCell {
        let newSettingsCell = LEFloatCell(style: .default, reuseIdentifier: nil)
        newSettingsCell.configureCell(title.localized(), CGFloat(value))
        return newSettingsCell
    }
    
    private static func makeStringInputCell(title: String, value: String?) -> LEStringCell {
        let newSettingsCell = LEStringCell(style: .default, reuseIdentifier: nil)
        newSettingsCell.configureCell(title.localized(), value)
        return newSettingsCell
    }
}
