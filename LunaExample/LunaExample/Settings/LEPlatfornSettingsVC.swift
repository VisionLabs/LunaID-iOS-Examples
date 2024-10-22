//
//  LEPlatfornSettingsVC.swift
//  LunaExample
//
//  Created by  Геращенко Андрей on 12.09.2024.
//

import UIKit
import LunaCore
import LunaCamera
import LunaWeb

class LEPlatfornSettingsVC: UIViewController, UITableViewDelegate, UITableViewDataSource {
    
    private enum PlatformSettings: Int, CaseIterable {
        case identifyHandlerID = 0
        case registrationHandlerID = 1
        case verifyID = 2
        case accountID = 3
        case platformURL = 4
        case platformToken = 5
    }
    
    private enum SettingsTitles: String {
        case identifyHandlerID = "settings.platform.identifyHandlerID_config"
        case registrationHandlerID = "settings.platform.registrationHandlerID_config"
        case verifyID = "settings.platform.verifyID_config"
        case lunaAccountID = "settings.platform.lunaAccountID_config"
        case accountID = "settings.platform.accountID_config"
        case platformURL = "settings.platform.URL_config"
        case platformToken = "settings.platform.token_config"
    }

    private var configuration = LunaCore.LCLunaConfiguration()
    private let tableView = UITableView(frame: .zero, style: .grouped)
    
    override func loadView() {
        super.loadView()
        self.createLayout()
    }
    
    public override func viewWillDisappear(_ animated: Bool) {
        navigationController?.navigationBar.isHidden = true
    }
    
    public override func viewWillAppear(_ animated: Bool) {
        navigationController?.navigationBar.isHidden = false
    }
    
    //  MARK: - UITableViewDelegate -
    
    func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return PlatformSettings.allCases.count;
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        var newCell: UITableViewCell?
        
        switch indexPath.row {
        case PlatformSettings.identifyHandlerID.rawValue:
            newCell = Self.makeStringInputCell(title: SettingsTitles.identifyHandlerID.rawValue,
                                               value: configuration.identifyHandlerID)
        case PlatformSettings.registrationHandlerID.rawValue:
            newCell = Self.makeStringInputCell(title: SettingsTitles.registrationHandlerID.rawValue,
                                               value: configuration.registrationHandlerID)
        case PlatformSettings.verifyID.rawValue:
            newCell = Self.makeStringInputCell(title: SettingsTitles.verifyID.rawValue,
                                               value: configuration.verifyID)
        case PlatformSettings.accountID.rawValue:
            newCell = Self.makeStringInputCell(title: SettingsTitles.accountID.rawValue,
                                               value: configuration.accountID)
        case PlatformSettings.platformURL.rawValue:
            newCell = Self.makeStringInputCell(title: SettingsTitles.platformURL.rawValue,
                                               value: configuration.lunaPlatformURL?.absoluteString)
        case PlatformSettings.platformToken.rawValue:
            newCell = Self.makeStringInputCell(title: SettingsTitles.platformToken.rawValue,
                                               value: configuration.platformToken)
        default:
            break
        }
        
        return newCell ?? UITableViewCell(frame: .zero)
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        switch indexPath.row {
        case PlatformSettings.identifyHandlerID.rawValue:
            let inputVC = LEInputVC(initialText: configuration.identifyHandlerID)
            inputVC.valueChangedHandler = { [weak self] text in
                guard let text, let self else { return }
                self.configuration.identifyHandlerID = text
                self.tableView.reloadData()
            }
            self.navigationController?.pushViewController(inputVC, animated: true)
        case PlatformSettings.registrationHandlerID.rawValue:
            let inputVC = LEInputVC(initialText: configuration.registrationHandlerID)
            inputVC.valueChangedHandler = { [weak self] text in
                guard let text, let self else { return }
                self.configuration.registrationHandlerID = text
                self.tableView.reloadData()
            }
            self.navigationController?.pushViewController(inputVC, animated: true)
        case PlatformSettings.verifyID.rawValue:
            let inputVC = LEInputVC(initialText: configuration.verifyID)
            inputVC.valueChangedHandler = { [weak self] text in
                guard let text, let self else { return }
                self.configuration.verifyID = text
                self.tableView.reloadData()
            }
            self.navigationController?.pushViewController(inputVC, animated: true)
        case PlatformSettings.accountID.rawValue:
            let inputVC = LEInputVC(initialText: configuration.accountID)
            inputVC.valueChangedHandler = { [weak self] text in
                guard let text, let self else { return }
                self.configuration.accountID = text
                self.tableView.reloadData()
            }
            self.navigationController?.pushViewController(inputVC, animated: true)
        case PlatformSettings.platformURL.rawValue:
                let inputVC = LEInputVC(initialText: configuration.lunaPlatformURL?.absoluteString)
            inputVC.valueChangedHandler = { [weak self] text in
                guard let self, let text,
                      let url = URL(string: text)
                    else { return }
                self.configuration.lunaPlatformURL = url
                self.tableView.reloadData()
            }
            self.navigationController?.pushViewController(inputVC, animated: true)
        case PlatformSettings.platformToken.rawValue:
            let inputVC = LEInputVC(initialText: configuration.platformToken)
            inputVC.valueChangedHandler = { [weak self] text in
                guard let text, let self else { return }
                self.configuration.platformToken = text
                self.tableView.reloadData()
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
