//
//  SettingsViewController.swift
//  LunaExample
//
//  Created by IVAN CHIRKOV on 27.10.2020.
//

import UIKit
import LunaCore
import LunaCamera

class SettingsViewController: UIViewController, UITableViewDelegate, UITableViewDataSource {
    
    private let CloseButtonSize: CGFloat = 44
    private let SideOffset: CGFloat = 10

    private let tableView = UITableView(frame: .zero, style: .grouped)
    
    public var configuration = LCLunaConfiguration.defaultConfig()
    
    override func loadView() {
        super.loadView()
        
        createLayout()
    }
    
    func numberOfSections(in tableView: UITableView) -> Int {
        return 2
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
        case 1:
            headerLabel.text = "Info about versions"
        default:
            break
        }
        
        return containerView
    }

    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return 1
    }
    
    private func findLivenessStatus() -> Bool {
        var status = false
        
        switch configuration.bestShotConfiguration.livenessType {
        case .byPhoto:
            status = true
        default:
            break
        }
        
        return status
    }
    
    private func setupLivenessStatus(_ livenessStatus: Bool) {
        if livenessStatus {
            configuration.bestShotConfiguration.livenessType = .byPhoto
        }
        else {
            configuration.bestShotConfiguration.livenessType = .none
        }
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        var newCell = UITableViewCell(frame: .zero)
        
        switch indexPath.section {
        case 0:
            let newSettingsCell = SettingsLivenessCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.livenessStatusHandler = { livenessStatus in
                self.setupLivenessStatus(livenessStatus)
            }
            newSettingsCell.configureCell(findLivenessStatus())
            newCell = newSettingsCell
        case 1:
            let newSettingsCell = SettingsVersionCell(style: .default, reuseIdentifier: nil)
            newSettingsCell.configureCell()
            newCell = newSettingsCell
        default:
            break
        }
        
        return newCell
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        switch indexPath.section {
        case 1:
            navigationController?.pushViewController(LEAboutVC(), animated: true)
        default:
            break
        }
    }
    
    private func createLayout() {
        view.backgroundColor = .white
        navigationItem.backButtonTitle = ""

        let closeButton = UIButton(frame: .zero)
        closeButton.translatesAutoresizingMaskIntoConstraints = false
        closeButton.setImage(UIImage(named: "close", in: Bundle(for: SettingsViewController.self), compatibleWith: nil), for: .normal)
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
    
    @objc
    private func closeViewController() {
        navigationController?.dismiss(animated: true)
    }
    
}
