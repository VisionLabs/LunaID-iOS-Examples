//
//  LEDocumentsFileListVC.swift
//  LunaExample
//
//  Created by Kirill Zhadaev on 12.04.2024.
//

import UIKit
import LunaCamera
import LunaCore
import LunaWeb

final class LEDocumentsFileListVC: UIViewController, UITableViewDelegate, UITableViewDataSource, UIImagePickerControllerDelegate, UINavigationControllerDelegate, LCBestShotDelegate {
    private let VerticalOffset: CGFloat = 10
    private let SideOffset: CGFloat = 10
    private let ApplyButtonHeight: CGFloat = 44

    private lazy var livenessAPI = LivenessAPIv6(config: self.webconfiguration,
                                                 compressionQuality: self.configuration.compressionQuality,
                                                 livenessQuality: self.configuration.bestShotConfiguration.livenessQuality) { [weak self] _ in
        guard let platformToken = self?.webconfiguration.platformToken else { return [:] }
        return [APIv6Constants.Headers.authorization.rawValue: platformToken]
    }

    private lazy var bestShotDetector: LCBestShotDetectorProtocol =  LCBestShotBuilder.build(with: self,
                                                                                             livenessAPI: livenessAPI,
                                                                                             configuration: configuration,
                                                                                             licenseConfig: LCLicenseConfig.userDefaults(),
                                                                                             singleFrameMode: true)

    private let pathExtension: String?
    private let completionMode: CompletionMode
    private let imagePicker = UIImagePickerController()
    private var configuration = LunaCore.LCLunaConfiguration()
    private var webconfiguration = LWConfig()
    private var bestShotCompletion: ((Result<LCBestShotModel, Error>) -> Void)?
    private var fileURLs: [URL] = []
    private var numberOfSections = 1

    private let tableView = UITableView(frame: .zero, style: .plain)

    init(pathExtension: String?, 
         configuration: LCLunaConfiguration? = nil,
         completionMode: CompletionMode) {
        self.pathExtension = pathExtension
        self.configuration = configuration ?? LCLunaConfiguration()
        self.completionMode = completionMode
        super.init(nibName: nil, bundle: nil)
    }

    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    override func loadView() {
        super.loadView()

        fileURLs = fetchFileURLs()
        createLayout()
        bestShotDetector.screenSize(view.bounds.size, edges: view.safeAreaInsets)
        if case .imageBestShot(let bestShotCompletion) = self.completionMode {
            self.bestShotCompletion = bestShotCompletion
            imagePicker.delegate = self
            imagePicker.sourceType = .photoLibrary
            self.numberOfSections = 2
        }
    }

    override func viewWillDisappear(_ animated: Bool) {
        navigationController?.navigationBar.isHidden = true
    }

    override func viewWillAppear(_ animated: Bool) {
        navigationController?.navigationBar.isHidden = false
    }

    // MARK: - LCBestShotDelegate -

    func detectionRect(_ rect: CGRect, inFrameSize frameSize: CGSize) {}

    func bestShot(_ bestShot: LCBestShotModel) -> Bool {
        DispatchQueue.main.async { [weak self] in
            self?.navigationController?.popViewController() {
                self?.bestShotCompletion?(.success(bestShot))
            }
        }

        return true
    }

    func multipartBestShots(_ bestShots: [LCBestShotModel]) -> Bool { false }

    func bestShotError(_ error: Error) {
        DispatchQueue.main.async { [weak self] in
            self?.navigationController?.popViewController() {
                self?.bestShotCompletion?(.failure(error))
            }
        }
    }

    func interactionsFinish(with interactionFrames: [LCInteractionFrameInfo]) {}

    //  MARK: - UITableViewDataSource -

    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        section == 0 ? fileURLs.count : 1
    }
    
    func numberOfSections(in tableView: UITableView) -> Int { self.numberOfSections }

    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let title = indexPath.section == 0 ? fileURLs[indexPath.row].lastPathComponent
                                           : "settings.pick_photo".localized()
        
        let cell = createCell(title: title)
        return cell
    }

    //  MARK: - UITableViewDelegate -

    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        if indexPath.section == 0 {
            let appliedFileURL = fileURLs[indexPath.row]
            
            switch completionMode {
                case .plistFiles(let completion):
                    navigationController?.popViewController() {
                        completion(appliedFileURL)
                    }
                case .imageBestShot:
                    guard let image = UIImage(contentsOfFile: appliedFileURL.path) else {
                        presentModalError("errors.invalid_file_format".localized())
                        return
                    }
                    
                    DispatchQueue.main.async { [weak self] in
                        self?.push(frame: image)
                    }
            }
        } else {
            self.present(imagePicker, animated: true)
        }
        self.tableView.deselectRow(at: indexPath, animated: true)
    }
    
    // MARK: - UIImagePickerControllerDelegate
    
    func imagePickerController(_ picker: UIImagePickerController, didFinishPickingMediaWithInfo info: [UIImagePickerController.InfoKey: Any]) {
        if let selectedImage = info[.originalImage] as? UIImage {
            DispatchQueue.main.async { [weak self] in
                self?.push(frame: selectedImage)
            }
        }
        self.dismiss(animated: true)
    }

    func imagePickerControllerDidCancel(_ picker: UIImagePickerController) {
        self.dismiss(animated: true)
    }

    //  MARK: - CompletionMode -

    enum CompletionMode {
        case plistFiles(completion: (URL) -> Void)
        case imageBestShot((Result<LCBestShotModel, Error>) -> Void)
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
            tableView.topAnchor.constraint(equalTo: view.topAnchor),
            tableView.leadingAnchor.constraint(equalTo: view.leadingAnchor),
            tableView.trailingAnchor.constraint(equalTo: view.trailingAnchor),
            tableView.bottomAnchor.constraint(equalTo: view.bottomAnchor),
        ])
    }

    private func fetchFileURLs() -> [URL] {
        let fileManager = FileManager.default
        var fileURLs: [URL] = []

        guard let documentsPath = fileManager.urls(for: .documentDirectory, in: .userDomainMask).first
        else { return [] }

        do {
            fileURLs = try fileManager.contentsOfDirectory(at: documentsPath,
                                                           includingPropertiesForKeys: nil,
                                                           options: [])

            if let pathExtension = pathExtension {
                fileURLs = fileURLs.filter { $0.pathExtension == pathExtension }
            }
        } catch let error {
            print("Could not find files in the Documents directory: \(error)")
        }

        return fileURLs
    }

    private func createCell(title: String) -> UITableViewCell {
        let cell = UITableViewCell()
        cell.contentView.backgroundColor = .clear

        let label = UILabel(frame: .zero)
        label.backgroundColor = .clear
        label.translatesAutoresizingMaskIntoConstraints = false
        label.font = Fonts.etelka_17
        label.textColor = .black
        label.text = title
        cell.contentView.addSubview(label)

        NSLayoutConstraint.activate([
            label.topAnchor.constraint(equalTo: cell.contentView.topAnchor, constant: SideOffset),
            label.leadingAnchor.constraint(equalTo: cell.contentView.leadingAnchor, constant: SideOffset),
            label.trailingAnchor.constraint(equalTo: cell.contentView.trailingAnchor, constant: -SideOffset),
            label.bottomAnchor.constraint(equalTo: cell.contentView.bottomAnchor, constant: -SideOffset),
        ])

        return cell
    }
    
    private func push(frame: UIImage) {
        DispatchQueue.main.async { [weak self] in
            self?.bestShotDetector.pushFrame(with: frame)
        }
    }
}

