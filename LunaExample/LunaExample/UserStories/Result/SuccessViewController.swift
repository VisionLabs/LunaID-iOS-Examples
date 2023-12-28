//
//  SuccessViewController.swift
//  LunaExample
//
//  Created by IVAN CHIRKOV on 09.11.2020.
//

import UIKit
import LunaWeb
import CoreLocation

class SuccessViewController: UIViewController {

    @IBOutlet weak var resultLabel: UILabel!
    @IBOutlet weak var userNameLabel: UILabel!
    
    var candidate: LunaWeb.APIv6.Face? = nil
    var location: CLLocation? = nil
    var isIdentify: Bool = false
    
    override func viewDidLoad() {
        super.viewDidLoad()
        if let userData = candidate?.userData {
            userNameLabel.text = "success.user_name".localized() + ": " + userData
        }
        resultLabel.text = isIdentify ? "success.identify_result".localized() : "success.verify_result".localized()

        if let location = location, let text = resultLabel.text {
            resultLabel.text =  text + "\n\n\(location.description)"
        }
    }
    
    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        navigationController?.setNavigationBarHidden(false, animated: true)
    }

}
