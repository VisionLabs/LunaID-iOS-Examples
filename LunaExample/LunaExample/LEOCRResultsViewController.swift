//
//  LEOCRViewController.swift
//  LunaExampleOCR
//
//  Created by michael on 25.10.2023.
//

import UIKit
import LunaCore
import LunaWeb
import LunaCamera
import OCR

class LEOCRResultsViewController: UIViewController, UITableViewDataSource {
    
    private let SideOffset: CGFloat = 16
    private let TitlesBetween: CGFloat = 2
    private let PhotosStackTopOffset: CGFloat = 24
    private let BottomButtonsHeight: CGFloat = 44

    private var ocrResult: OCR.OCRResult?
    private var bestShot: LunaCore.LCBestShot?
    
    private var ocrFieldsToShow = [OCRResultTextField]()
    private var continueTitle = ""

    private let activityIndicator = LEActivityIndicatorView(frame: .zero)
    private let tableView = UITableView(frame: .zero, style: .plain)
    private let continueButton = LCRoundButton(type: .custom)
    
    public var continueButtonHandler: ((Error?) -> Void)?
    public var retryBiometricHandler: VoidHandler?
    public var retryOCRHandler: VoidHandler?
    
    public var configuration: LCLunaConfiguration = LCLunaConfiguration()
    
    override func loadView() {
        super.loadView()
        
        createLayout()
    }

    public func configureResults(_ bestShot: LunaCore.LCBestShot, _ ocrResult: OCR.OCRResult?, _ title: String) {
        self.bestShot = bestShot
        self.ocrResult = ocrResult

        if let ocrFields = ocrResult?.textFields {
            self.ocrFieldsToShow = preparePresentationValues(from: ocrFields)
        }
        
        self.continueTitle = title
        
        tableView.reloadData()
    }
    
    //  MARK: - Handlers -

    @objc
    private func continueButtonTapped(_ sender: UIButton) {
        activityIndicator.startAnimating()
        guard let ocrResult = ocrResult,
              !ocrResult.isEmpty() else {
            continueButtonHandler?(OCRError.notRecognized)
            activityIndicator.stopAnimating()
            return
        }
        
        let faceDetector = LCFaceDetectorBuilder.build(with: configuration)
        guard let imageField = ocrResult.faceImageField() else {
            continueButtonHandler?(LEAuthError.faceOnDocumentNotFound)
            activityIndicator.stopAnimating()
            return
        }
        guard let detection = faceDetector.detectFaces(imageField.image, maxCount: 2).first else {
            continueButtonHandler?(LEAuthError.faceOnDocumentNotFound)
            activityIndicator.stopAnimating()
            return
        }

        let extractor = LCDescriptorExtractorBuilder.build(with: configuration)
        let currentMatchValue: CGFloat = CGFloat(extractor.match(bestShot!, and: detection))
        if (currentMatchValue < configuration.documentVerificationMatch) {
            continueButtonHandler?(LEAuthError.documentVerificationError)
            activityIndicator.stopAnimating()
            return
        }

        continueButtonHandler?(nil)
        activityIndicator.stopAnimating()
    }

    //  MARK: - UITableViewDataSource -

    func numberOfSections(in tableView: UITableView) -> Int {
        return 1;
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        ocrFieldsToShow.count + 1
    }

    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        switch indexPath.row {
        case 0:
            let cell = tableView.dequeueReusableCell(withIdentifier: LEPhotosCell.reuseID) as! LEPhotosCell
            if let bestShot = bestShot,
               let ocrResult = ocrResult {
                cell.configureCell(bestShot.getUIImageWarped(true), ocrResult.faceImageField()?.image)
            }
            cell.retryOCRHandler = retryOCRHandler
            cell.retryBiometricHandler = retryBiometricHandler
            return cell
            
        default:
            let cell = tableView.dequeueReusableCell(withIdentifier: LEOCRFieldCell.reuseID, for: indexPath) as! LEOCRFieldCell
            let typeName = ocrFieldsToShow[indexPath.row - 1].typeName
            var isValueChangeable = false
            let typeValue: String
            if typeName == "given_name".localized() {
                typeValue = ocrFieldsToShow[indexPath.row - 1].value
            } else {
                typeValue = hideSensitiveData(inputString: ocrFieldsToShow[indexPath.row - 1].value)
            }
            cell.configureCell(typeName, typeValue, isValueChangeable: isValueChangeable)
            return cell
        }
    }

    func tableView(_ tableView: UITableView, heightForRowAt indexPath: IndexPath) -> CGFloat {
        UITableView.automaticDimension
    }

    //  MARK: - Routine -
    
    private func createLayout() {
        view.backgroundColor = .white
        
        tableView.translatesAutoresizingMaskIntoConstraints = false
        tableView.allowsSelection = false
        tableView.separatorStyle = .none
        tableView.dataSource = self
        tableView.showsVerticalScrollIndicator = false
        tableView.register(LEOCRFieldCell.self, forCellReuseIdentifier: LEOCRFieldCell.reuseID)
        tableView.register(LEPhotosCell.self, forCellReuseIdentifier: LEPhotosCell.reuseID)
        view.addSubview(tableView)
        
        let instructionsTitleLabel = createLabel()
        instructionsTitleLabel.font = Fonts.etelka_17
        instructionsTitleLabel.text = "capture_result.check_results".localized()
        view.addSubview(instructionsTitleLabel)
        
        let instructionsSubtitleLabel = createLabel()
        instructionsSubtitleLabel.font = Fonts.etelka_17
        instructionsSubtitleLabel.text = "capture_result.retry_or_continue".localized()
        view.addSubview(instructionsSubtitleLabel)
        
        continueButton.translatesAutoresizingMaskIntoConstraints = false
        continueButton.setTitle(continueTitle, for: .normal)
        continueButton.addTarget(self, action: #selector(continueButtonTapped), for: .touchUpInside)
        view.addSubview(continueButton)

        activityIndicator.translatesAutoresizingMaskIntoConstraints = false
        view.addSubview(activityIndicator)

        NSLayoutConstraint.activate([
            instructionsTitleLabel.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor),
            instructionsTitleLabel.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: SideOffset),
            instructionsTitleLabel.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -SideOffset),
            
            instructionsSubtitleLabel.topAnchor.constraint(equalTo: instructionsTitleLabel.bottomAnchor, constant: TitlesBetween),
            instructionsSubtitleLabel.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: SideOffset),
            instructionsSubtitleLabel.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -SideOffset),
            
            tableView.topAnchor.constraint(equalTo: instructionsSubtitleLabel.bottomAnchor, constant: PhotosStackTopOffset),
            tableView.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: SideOffset),
            tableView.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -SideOffset),
            tableView.bottomAnchor.constraint(equalTo: continueButton.topAnchor, constant: -PhotosStackTopOffset),
            
            continueButton.heightAnchor.constraint(equalToConstant: BottomButtonsHeight),
            continueButton.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: SideOffset),
            continueButton.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -SideOffset),
            continueButton.bottomAnchor.constraint(equalTo: view.safeAreaLayoutGuide.bottomAnchor),

            activityIndicator.leadingAnchor.constraint(equalTo: view.leadingAnchor),
            activityIndicator.trailingAnchor.constraint(equalTo: view.trailingAnchor),
            activityIndicator.topAnchor.constraint(equalTo: view.topAnchor),
            activityIndicator.bottomAnchor.constraint(equalTo: view.bottomAnchor),
        ])
    }
    
    private func createLabel() -> UILabel {
        let newLabel = UILabel(frame: .zero)
        
        newLabel.translatesAutoresizingMaskIntoConstraints = false
        newLabel.numberOfLines = 0
        newLabel.backgroundColor = .clear
        
        return newLabel
    }

    private func preparePresentationValues(from resultFields: [OCRResultTextField]) -> [OCRResultTextField] {
        var givenNameTextField = resultFields.first(where: { $0.typeName == "Given_Names" })
        var fathersNameTextField = resultFields.first(where: { $0.typeName == "Fathers_Name" })
        var documentNumberTextField = resultFields.first(where: { $0.typeName == "Document_Number" })

        givenNameTextField?.typeName = "given_name".localized()
        fathersNameTextField?.typeName = "fathers_name".localized()
        documentNumberTextField?.typeName = "document_id".localized()

        return [givenNameTextField, fathersNameTextField, documentNumberTextField].compactMap { $0 }
    }

    private func hideSensitiveData(inputString: String) -> String {
        let firstCharacter = inputString.prefix(1)
        let lastCharacter = inputString.suffix(1)
        let hiddenPart = String(repeating: "*", count: inputString.count - 2)
        
        return "\(firstCharacter)\(hiddenPart)\(lastCharacter)"
    }

    private func closeViewController(animated: Bool) {
        navigationController?.popViewController(animated: animated)
    }
    
}
