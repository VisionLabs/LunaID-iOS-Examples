//
//  FailViewController.swift
//  LunaExample
//
//  Created by IVAN CHIRKOV on 09.11.2020.
//

import UIKit
import CoreLocation

class FailViewController: UIViewController {

    @IBOutlet weak var resultLabel: UILabel!
    
    var isIdentify: Bool = false
    var location: CLLocation?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        resultLabel.text = isIdentify ? "fail.identify_result".localized() : "fail.verify_result".localized()
        if let location = location, let text = resultLabel.text {
            resultLabel.text = text + "\n\n\(location.description)"
        }
    }
    
    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        navigationController?.setNavigationBarHidden(false, animated: true)
    }

}
