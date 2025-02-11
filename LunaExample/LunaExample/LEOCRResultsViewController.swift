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
    enum Scenario {
        case registration, verification, identification
    }

    private let SideOffset: CGFloat = 16
    private let TitlesBetween: CGFloat = 2
    private let PhotosStackTopOffset: CGFloat = 24
    private let BottomButtonsHeight: CGFloat = 44

    private var ocrResult: OCR.OCRResult?
    private var bestShot: LunaCore.LCBestShotModel?
    private var face: APIv6.Face?
    private var scenario: Scenario = .registration

    private var ocrFieldsToShow = [OCRResultTextField]()
    private var continueTitle = ""

    private let activityIndicator = LEActivityIndicatorView(frame: .zero)
    private let tableView = UITableView(frame: .zero, style: .plain)
    private let continueButton = LCRoundButton(type: .custom)
    
    public var continueButtonHandler: ((Error?) -> Void)?
    public var retryBiometricHandler: ((@escaping (LunaCore.LCBestShotModel?) -> Void) -> Void)?
    public var retryOCRHandler: ((@escaping (OCR.OCRResult?) -> Void) -> Void)?

    public var configuration: LCLunaConfiguration = LCLunaConfiguration()

    private var lunaAPI: LunaWeb.APIv6 = {
        LunaWeb.APIv6(lunaAccountID: LCLunaConfiguration().lunaAccountID,
                      lunaServerURL: LCLunaConfiguration().lunaPlatformURL) { _ in
            guard let platformToken = LCLunaConfiguration().platformToken else { return [:] }
            return [APIv6Constants.Headers.authorization.rawValue: platformToken]
        }
    }()

    private var passportData: [String: EventQuery.PassportData] {
        var result: [String: EventQuery.PassportData] = [:]

        ocrResult?.textFields.forEach { ocrResultTextField in
            result[ocrResultTextField.typeName.lowercased()] = EventQuery.PassportData(
                title: ocrResultTextField.localizedTypeName ?? "",
                value: ocrResultTextField.value
            )
        }

        return result
    }

    override func loadView() {
        super.loadView()
        
        createLayout()
    }

    public func configureResults(scenario: Scenario, _ bestShot: LunaCore.LCBestShotModel, _ ocrResult: OCR.OCRResult?, _ face: APIv6.Face) {
        self.scenario = scenario
        self.bestShot = bestShot
        self.ocrResult = ocrResult
        self.face = face

        if let ocrFields = ocrResult?.textFields {
            self.ocrFieldsToShow = preparePresentationValues(from: ocrFields)
        }
        switch scenario {
            case .registration:
                self.continueTitle = "buttons.do_actions.registration".localized()
            case .verification:
                self.continueTitle = "buttons.do_actions.verify".localized()
            case .identification:
                self.continueTitle = "buttons.do_actions.identify".localized()
        }
        
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
        
        guard let imageField = ocrResult.faceImageField() else {
            continueButtonHandler?(LEAuthError.faceOnDocumentNotFound)
            activityIndicator.stopAnimating()
            return
        }
        
        let lunaIDService: LunaCore.LCLunaIDServiceProtocol = LunaCore.LCLunaIDService(config: configuration,
                                                                                       licenseConfig: LunaCore.LCLicenseConfig.userDefaults())
        guard let detection = lunaIDService.detectFaces(imageField.image).first else {
            continueButtonHandler?(LEAuthError.faceOnDocumentNotFound)
            activityIndicator.stopAnimating()
            return
        }

        let currentMatchValue = lunaIDService.match(bestShot!, faceDetection: detection)
        if (currentMatchValue < configuration.documentVerificationMatch) {
            continueButtonHandler?(LEAuthError.documentVerificationError)
            activityIndicator.stopAnimating()
            return
        }
        
        let completionBlock: (Error?) -> Void = { [weak self] error in
            DispatchQueue.main.async {
                guard let self else { return }
                self.continueButtonHandler?(error)
                self.activityIndicator.stopAnimating()
            }
        }

        switch scenario {
        case .registration:
            signUpUser(with: ocrResult, completion: completionBlock)
        case .verification, .identification:
            handleUser(by: scenario, with: ocrResult, completion: completionBlock)
        }
    }

    private func signUpUser(with ocrResult: OCR.OCRResult, completion: @escaping (Error?) -> Void) {
        guard let result = ocrResult as? OCRRegulaResult,
              let documentImageContainer = result.imageFields.first(where: { $0.typeName == "DocumentImage" }),
              let imageData = documentImageContainer.image.jpegData(compressionQuality: 1.0),
              !passportData.isEmpty
        else {
            completion(OCRError.notRecognized)
            return
        }

        let requestId = UUID().uuidString

        lunaAPI.images.createImage(imageData: imageData, requestId: requestId) { [weak self] result in
            guard let self else { return }
            switch result {
            case .success(let createImageResponse):
                let meta = EventQuery.Meta(
                    schemaName: "mali",
                    schemaVersion: 3,
                    passportSavedImages: [createImageResponse],
                    passportData: passportData
                )

                generateEvents(meta: meta, completion: completion)
            case .failure(let error):
                completion(OCRError.error(error))
            }
        }
    }
    
    private func handleUser(by scenario: Scenario, with ocrResult: OCR.OCRResult,
                            completion: @escaping (Error?) -> Void) {
        guard let faceID = self.face?.id else { completion(LEAuthError.faceOnDocumentNotFound); return }
        let query = GetDocumentsQuery(faceID: faceID)
        lunaAPI.events.getDocumentEvents(query: query) { [weak self] result in
            guard let self else { return }
            switch result {
            case .success(let response):
                let handleError: LEAuthError = scenario == .verification
                                                        ? .verificationError : .identificationError
                let error = self.isSame(document: ocrResult, to: response) ? nil : handleError
                completion(error)
            case .failure(let error):
                completion(error)
            }
        }
    }

    private func generateEvents(meta: EventQuery.Meta, completion: @escaping (Error?) -> Void) {
        guard let bestShotData: LunaWeb.BestShotData = bestShot?.bestShotData(configuration: configuration, isWarped: true)
        else {
            completion(OCRError.notRecognized)
            return
        }

        let query = EventQuery(bestShotsData: [bestShotData],
                               imageType: .faceWarpedImage,
                               externalID: face?.userData,
                               userData: meta.passportData["surname_and_given_names"]?.value,
                               meta: meta)

        lunaAPI.events.generateEvents(handlerID: configuration.registrationHandlerID, query: query) { result in
            switch result {
            case .success(let response):
                let error = response.events.first?.face == nil ? LEAuthError.userAlreadyExists : nil
                completion(error)
            case .failure(let error):
                completion(LEAuthError.error(error))
            }
        }
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

            cell.retryOCRHandler = { [weak self] in
                self?.retryOCRHandler? { ocrResult in
                    // Закрываем LEOCRViewController
                    self?.navigationController?.popViewController(animated: true) {
                        self?.ocrResult = ocrResult
                        self?.tableView.reloadData()
                    }
                }
            }

            cell.retryBiometricHandler = { [weak self] in
                self?.retryBiometricHandler? { bestShot in
                    // Закрываем IdentifyViewController
                    self?.navigationController?.popViewController(animated: true) {
                        self?.bestShot = bestShot
                        self?.tableView.reloadData()
                    }
                }
            }
            
            return cell
            
        default:
            let cell = tableView.dequeueReusableCell(withIdentifier: LEOCRFieldCell.reuseID, for: indexPath) as! LEOCRFieldCell
            let typeName = ocrFieldsToShow[indexPath.row - 1].typeName
            let typeValue: String
            if typeName == "given_name".localized() {
                typeValue = ocrFieldsToShow[indexPath.row - 1].value
            } else {
                typeValue = hideSensitiveData(inputString: ocrFieldsToShow[indexPath.row - 1].value)
            }
            cell.configureCell(typeName, typeValue, isValueChangeable: false)
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
    
    private func isSame(document: OCRResult, to response: GetEventResponse) -> Bool {
        let documentNumberField = document.textFields.first(where: { $0.typeName == "Document_Number" })
        let documentNumber = Int(documentNumberField?.value ?? "") ?? -1
        let responseDocNumberField = response.events?.first?.meta?.passportData?.documentNumber?.value
        let responseDocNumber = Int(responseDocNumberField ?? "") ?? -2
        return responseDocNumber == documentNumber
    }
    
}
