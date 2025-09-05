//
//  LEAboutVC.swift
//  LunaExample
//
//  Created by cipher on 26.01.2023.
//

import UIKit
import LunaCore

public
class LEAboutVC: UIViewController { 
    
    private let SideOffset: CGFloat = 20
    private let SwitchSize: CGSize = CGSize(width: 50, height: 30)
    
    public var lunaConfiguration = LCLunaConfiguration()

    public init(lunaConfig: LCLunaConfiguration) {
        super.init(nibName: nil, bundle: nil)
        
        self.lunaConfiguration = lunaConfig
    }

    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    public override func loadView() {
        super.loadView()
        
        createLayout()
    }

    public override func viewWillDisappear(_ animated: Bool) {
        navigationController?.navigationBar.isHidden = true
    }
    
    public override func viewWillAppear(_ animated: Bool) {
        navigationController?.navigationBar.isHidden = false
    }

    private func createLayout() {
        view.backgroundColor = .white

        let appVersionLabel = UILabel(frame: .zero)
        appVersionLabel.translatesAutoresizingMaskIntoConstraints = false
        appVersionLabel.text = "\(Bundle.main.infoDictionary?["CFBundleName"] as? String ?? "unknown") v.\(appVersion())"
        appVersionLabel.font = UIFont(name: "EtelkaMediumPro", size: 17)
        view.addSubview(appVersionLabel)
        
        let lunaIDSDKVersionLabel = UILabel(frame: .zero)
        lunaIDSDKVersionLabel.translatesAutoresizingMaskIntoConstraints = false
        lunaIDSDKVersionLabel.text = "LunaID SDK v.\(lunaConfiguration.lunaIDSDKVersion())"
        lunaIDSDKVersionLabel.font = UIFont(name: "EtelkaMediumPro", size: 17)
        view.addSubview(lunaIDSDKVersionLabel)
        
        let lunaSDKVersionLabel = UILabel(frame: .zero)
        lunaSDKVersionLabel.translatesAutoresizingMaskIntoConstraints = false
        lunaSDKVersionLabel.text = "Luna SDK v.\(lunaConfiguration.lunaSDKVersion())"
        lunaSDKVersionLabel.font = UIFont(name: "EtelkaMediumPro", size: 17)
        view.addSubview(lunaSDKVersionLabel)


        NSLayoutConstraint.activate([
            appVersionLabel.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor, constant: SideOffset),
            appVersionLabel.leadingAnchor.constraint(equalTo: view.leadingAnchor, constant: SideOffset),
            appVersionLabel.trailingAnchor.constraint(equalTo: view.trailingAnchor, constant: SideOffset),
            
            lunaIDSDKVersionLabel.topAnchor.constraint(equalTo: appVersionLabel.bottomAnchor, constant: SideOffset),
            lunaIDSDKVersionLabel.leadingAnchor.constraint(equalTo: view.leadingAnchor, constant: SideOffset),
            lunaIDSDKVersionLabel.trailingAnchor.constraint(equalTo: view.trailingAnchor, constant: SideOffset),
            
            lunaSDKVersionLabel.topAnchor.constraint(equalTo: lunaIDSDKVersionLabel.bottomAnchor, constant: SideOffset),
            lunaSDKVersionLabel.leadingAnchor.constraint(equalTo: view.leadingAnchor, constant: SideOffset),
            lunaSDKVersionLabel.trailingAnchor.constraint(equalTo: view.trailingAnchor, constant: SideOffset)
        ])
    }
    
    private func appVersion() -> String {
        let versionNumber = Bundle.main.infoDictionary?["CFBundleShortVersionString"] as! String
        let buildNumber = Bundle.main.infoDictionary?["CFBundleVersion"] as! String
        

        return "\(versionNumber)(\(buildNumber))"
    }

}
