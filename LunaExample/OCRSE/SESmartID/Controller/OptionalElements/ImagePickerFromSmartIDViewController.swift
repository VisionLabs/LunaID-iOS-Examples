//
//  GalleryButton.swift
//  SESmartIDSample
//
//  Created by Artuhay on 17.07.2020.
//  Copyright © 2020 biz.smartengines. All rights reserved.
//

import Foundation

class ImagePickerFromSmartIDViewController : UIImagePickerController {
	
	let pickerIAIContainerBackground : UIView = {
		let pickerIAIContainerBackground = UIView()
		pickerIAIContainerBackground.alpha = 0.2
		pickerIAIContainerBackground.backgroundColor = .gray
		pickerIAIContainerBackground.isUserInteractionEnabled = false
		pickerIAIContainerBackground.isHidden = true

		return pickerIAIContainerBackground
	}()
	
	var pickerImageActivityIndicatorContainer : UIView = {
		let activityContainer = UIView()
		activityContainer.backgroundColor = .black
		activityContainer.alpha = 0.8
		activityContainer.layer.cornerRadius = 10
		
		return activityContainer
	}()
	
	let pickerImageActivityIndicator : UIActivityIndicatorView = {
		let pickerImageActivityIndicator = UIActivityIndicatorView()
        pickerImageActivityIndicator.style = .whiteLarge
		pickerImageActivityIndicator.color = .red
		return pickerImageActivityIndicator
	}()
	
	func configureImagePickerComponents() {
		self.view.addSubview(pickerIAIContainerBackground)
		pickerIAIContainerBackground.translatesAutoresizingMaskIntoConstraints = false
		pickerIAIContainerBackground.centerXAnchor.constraint(equalTo: self.view.centerXAnchor).isActive = true
		pickerIAIContainerBackground.centerYAnchor.constraint(equalTo: self.view.centerYAnchor).isActive = true
		pickerIAIContainerBackground.widthAnchor.constraint(equalToConstant: UIScreen.main.bounds.width).isActive = true
		pickerIAIContainerBackground.heightAnchor.constraint(equalToConstant: UIScreen.main.bounds.height).isActive = true
		
		self.view.addSubview(pickerImageActivityIndicatorContainer)
		pickerImageActivityIndicatorContainer.translatesAutoresizingMaskIntoConstraints = false
		pickerImageActivityIndicatorContainer.centerXAnchor.constraint(equalTo: self.view.centerXAnchor).isActive = true
		pickerImageActivityIndicatorContainer.centerYAnchor.constraint(equalTo: self.view.centerYAnchor).isActive = true
		let activityWidth = min(UIScreen.main.bounds.width, UIScreen.main.bounds.height)/5
		pickerImageActivityIndicatorContainer.widthAnchor.constraint(equalToConstant: activityWidth).isActive = true
		pickerImageActivityIndicatorContainer.heightAnchor.constraint(equalToConstant: activityWidth).isActive = true
		pickerImageActivityIndicatorContainer.isHidden = true
		
		pickerImageActivityIndicatorContainer.addSubview(pickerImageActivityIndicator)
		pickerImageActivityIndicatorContainer.center  = pickerImageActivityIndicator.center
		pickerImageActivityIndicator.translatesAutoresizingMaskIntoConstraints = false
		pickerImageActivityIndicator.centerXAnchor.constraint(equalTo:pickerImageActivityIndicatorContainer.centerXAnchor).isActive = true
		pickerImageActivityIndicator.centerYAnchor.constraint(equalTo:pickerImageActivityIndicatorContainer.centerYAnchor).isActive = true
	}
	
	func setupImagePickerActivity() {
			self.pickerIAIContainerBackground.isHidden = false
			self.pickerImageActivityIndicatorContainer.isHidden = false
			self.pickerImageActivityIndicator.isHidden = false
	}
	
	override func viewDidLoad() {
		super.viewDidLoad()
		self.sourceType = .photoLibrary
		self.modalPresentationStyle = .fullScreen
		self.configureImagePickerComponents()
	}
	
}

@objc extension SmartIDViewController : UIImagePickerControllerDelegate, UINavigationControllerDelegate {
	
	func configureGalleryButton() {
		
		self.galleryButton = UIButton(type: .roundedRect)
		if let gButton = galleryButton {
			self.view.addSubview(gButton)
			
			gButton.translatesAutoresizingMaskIntoConstraints = false
			let buttonLayouts = [NSLayoutConstraint(item: gButton, attribute: .leading, relatedBy: .equal, toItem: self.view, attribute: .leading, multiplier: 1, constant: 25), NSLayoutConstraint(item: gButton, attribute: .bottom, relatedBy: .equal, toItem: self.view, attribute: .bottom, multiplier: 1, constant: -25)]
			let buttonConstants = [NSLayoutConstraint(item: gButton, attribute: .height, relatedBy: .equal, toItem: nil, attribute: .notAnAttribute, multiplier: 1, constant: 50), NSLayoutConstraint(item: gButton, attribute: .width, relatedBy: .equal, toItem: nil, attribute: .notAnAttribute, multiplier: 1, constant: 50)]
			self.view.addConstraints(buttonLayouts)
			gButton.addConstraints(buttonConstants)
			
			gButton.setTitle("G", for: .normal)
			gButton.titleLabel?.font = UIFont.boldSystemFont(ofSize: 30)
			gButton.titleLabel?.textColor = .cyan
			
			gButton.addTarget(self, action: #selector(showGalleryImagePickerToProcessImage), for: .touchUpInside)
		}
	}
	
	@objc func showGalleryImagePickerToProcessImage() {
		photoLibImagePicker = ImagePickerFromSmartIDViewController()
		if let imgPicker = photoLibImagePicker {
				imgPicker.delegate = self
				DispatchQueue.main.async {
					imgPicker.pickerIAIContainerBackground.isHidden = true
					imgPicker.pickerImageActivityIndicatorContainer.isHidden = true
				}

				self.present(imgPicker, animated: true, completion: nil)
		}
	}
	
	func pickImageByUIImage(image: UIImage) {
			//DispatchQueue.global(qos: .background).async { [weak self] in
		photoLibImagePicker?.setupImagePickerActivity()
		photoLibImagePicker?.pickerImageActivityIndicator.startAnimating()
		DispatchQueue.main.async { [weak self] in

			self?.processUIImageFile(image)
			//self?.smartIDController.processUIImageFile(image)
			self?.photoLibImagePicker?.pickerImageActivityIndicator.stopAnimating()
		}
		 // }
	}
	
	func imagePickerController(_ picker: UIImagePickerController, didFinishPickingMediaWithInfo info: [String : Any]) {
        pickImageByUIImage(image: info[UIImagePickerController.InfoKey.originalImage.rawValue] as! UIImage)
	}
	
    public func imagePickerControllerDidCancel(_ picker: UIImagePickerController) {
		dismiss(animated: true, completion: nil)
	}
}
