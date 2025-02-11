//
//  LEInteractionsSettingsVC.swift
//  LunaExample
//
//  Created by IVAN CHIRKOV on 27.10.2020.
//

import UIKit
import LunaCore
import LunaCamera
import LunaWeb

enum EInteractionsSettingsItem: Int {
    
    case INTERACTIONSSETTING_ITEM_DELAYBETWEEN
    case INTERACTIONSSETTING_ITEM_TIMEOUT
    case INTERACTIONSSETTING_ITEM_STEPS_NUMBER

    case INTERACTIONSSETTING_ITEM_COUNT
    
}

class LEInteractionsSettingsVC: UIViewController, UITableViewDelegate, UITableViewDataSource {
    
    private let SideOffset: CGFloat = 10
    private var config = LunaCore.LCLunaConfiguration.userDefaults()
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
            config.save()
        }
    }

    //  MARK: - UITableViewDelegate -
    
    func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return EInteractionsSettingsItem.INTERACTIONSSETTING_ITEM_COUNT.rawValue;
    }
    
    func tableView(_ tableView: UITableView, viewForFooterInSection section: Int) -> UIView? {
        return nil
    }
    
    func tableView(_ tableView: UITableView, viewForHeaderInSection section: Int) -> UIView? {
        return nil
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        var newCell = UITableViewCell(frame: .zero)
        
        var settingsItem = EInteractionsSettingsItem(rawValue: indexPath.row)
        switch settingsItem {
        case .INTERACTIONSSETTING_ITEM_DELAYBETWEEN:
            let newSettingsCell = LEFloatCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.delay_between".localized(), CGFloat(config.interactionsConfig.timeoutBetweenInteractions))
            newCell = newSettingsCell
        case .INTERACTIONSSETTING_ITEM_TIMEOUT:
            let newSettingsCell = LEFloatCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.interaction_timeout".localized(), CGFloat(config.interactionsConfig.interactionTimeout))
            newCell = newSettingsCell
        case .INTERACTIONSSETTING_ITEM_STEPS_NUMBER:
            let newSettingsCell = LEFloatCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell("settings.steps_number".localized(), CGFloat(config.interactionsConfig.stepsNumber))
            newCell = newSettingsCell
        default:
            break
        }
        
        return newCell
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        let settingsItem = EInteractionsSettingsItem(rawValue: indexPath.row)
        switch settingsItem {
        case .INTERACTIONSSETTING_ITEM_DELAYBETWEEN:
            showDurationPicker("settings.delay_between".localized(), CGFloat(config.interactionsConfig.timeoutBetweenInteractions), 1000, 1, { newTimeout in
                self.config.interactionsConfig.timeoutBetweenInteractions = newTimeout
                self.tableView.reloadData()
            })
        case .INTERACTIONSSETTING_ITEM_TIMEOUT:
            showDurationPicker("settings.interaction_timeout".localized(), CGFloat(config.interactionsConfig.interactionTimeout), 1000, 1, { newTimeout in
                self.config.interactionsConfig.interactionTimeout = newTimeout
                self.tableView.reloadData()
            })
        case .INTERACTIONSSETTING_ITEM_STEPS_NUMBER:
            showDurationPicker("settings.steps_number".localized(), CGFloat(config.interactionsConfig.stepsNumber), 6, 1, { newNumber in
                self.config.interactionsConfig.stepsNumber = Int(newNumber)
                self.tableView.reloadData()
            })
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
    
    private func showDurationPicker(_ title: String, _ initialValue: CGFloat, _ limit: CGFloat, _ limitPower: CGFloat, _ changeHandler: @escaping DurationChangedHandler) {
        let pickerVC = LEDurationPickerVC(title: title, limit: limit, limitPower: limitPower, initialValue: initialValue)
        pickerVC.periodChangedHandler = changeHandler
        view.addSubview(pickerVC.view)
        addChild(pickerVC)
    }

}
