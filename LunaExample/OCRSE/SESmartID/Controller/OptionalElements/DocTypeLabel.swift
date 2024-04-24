//
//  DocTypeLabel.swift
//  SESmartIDSample
//
//  Created by Artuhay on 16.07.2020.
//  Copyright © 2020 biz.smartengines. All rights reserved.
//

import UIKit

@objc public class DocTypeLabel : UILabel {
	
    init() {
		super.init(frame: .zero)
		self.backgroundColor = UIColor.black.withAlphaComponent(0.8)
		self.textAlignment = .center
		self.textColor = .white
	}
	
	required init?(coder: NSCoder) {
		fatalError("init(coder:) has not been implemented")
	}
}

extension SmartIDViewController {
	@objc public func configureDocumentTypeLabel(_ typeName: String) {
		if docTypeLabel == nil {
			docTypeLabel = DocTypeLabel()
		}
		if let docTypeL = docTypeLabel {
			docTypeL.text = typeName
			self.view.addSubview(docTypeL)
			docTypeL.translatesAutoresizingMaskIntoConstraints = false
			if #available(iOS 11, *) {
				docTypeL.trailingAnchor.constraint(equalTo: self.view.safeAreaLayoutGuide.trailingAnchor, constant: 0).isActive = true
				docTypeL.leadingAnchor.constraint(equalTo: self.view.safeAreaLayoutGuide.leadingAnchor, constant: 0).isActive = true
				docTypeL.topAnchor.constraint(equalTo: self.view.safeAreaLayoutGuide.topAnchor, constant: 0).isActive = true
			} else {
				docTypeL.trailingAnchor.constraint(equalTo: self.view.trailingAnchor, constant: 0).isActive = true
				docTypeL.leadingAnchor.constraint(equalTo: self.view.leadingAnchor, constant: 0).isActive = true
				docTypeL.topAnchor.constraint(equalTo: self.view.topAnchor, constant: 0).isActive = true
			}
			docTypeL.heightAnchor.constraint(equalToConstant: 50).isActive = true
		}
	}
	
}
