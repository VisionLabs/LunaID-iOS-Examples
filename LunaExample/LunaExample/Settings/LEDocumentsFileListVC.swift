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

final class LEDocumentsFileListVC: UIViewController, UITableViewDelegate, UITableViewDataSource, LCBestShotDelegate {
    private let VerticalOffset: CGFloat = 10
    private let SideOffset: CGFloat = 10
    private let ApplyButtonHeight: CGFloat = 44

    private lazy var livenessAPI = LivenessAPIv6(configuration: configuration) { [weak self] _ in
        guard let platformToken = self?.configuration.platformToken else { return [:] }
        return [APIv6Constants.Headers.authorization.rawValue: platformToken]
    }

    private lazy var bestShotDetector: LCBestShotDetectorProtocol = LCBestShotBuilder.build(with: self,
                                                                                            livenessAPI: livenessAPI,
                                                                                            configuration: configuration,
                                                                                            isUserDefaultsPillar: true,
                                                                                            singleFrameMode: true,
                                                                                            licenseBundleID: Bundle.main.bundleIdentifier ?? "")

    private let pathExtension: String?
    private let completionMode: CompletionMode
    private var configuration = LunaCore.LCLunaConfiguration()
    private var bestShotCompletion: ((Result<LCBestShot, Error>) -> Void)?
    private var fileURLs: [URL] = []

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
    }

    override func viewWillDisappear(_ animated: Bool) {
        navigationController?.navigationBar.isHidden = true
    }

    override func viewWillAppear(_ animated: Bool) {
        navigationController?.navigationBar.isHidden = false
    }

    // MARK: - LCBestShotDelegate -

    func detectionRect(_ rect: CGRect, inFrameSize frameSize: CGSize) {}

    func bestShot(_ bestShot: LCBestShot) -> Bool {
        DispatchQueue.main.async { [weak self] in
            self?.navigationController?.popViewController() {
                self?.bestShotCompletion?(.success(bestShot))
            }
        }

        return true
    }

    func multipartBestShots(_ bestShots: [LCBestShot]) -> Bool { false }

    func bestShotError(_ error: Error) {
        DispatchQueue.main.async { [weak self] in
            self?.navigationController?.popViewController() {
                self?.bestShotCompletion?(.failure(error))
            }
        }
    }

    //  MARK: - UITableViewDataSource -

    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        fileURLs.count
    }

    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let title = fileURLs[indexPath.row].lastPathComponent
        let cell = createCell(title: title)
        return cell
    }

    //  MARK: - UITableViewDelegate -

    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        let appliedFileURL = fileURLs[indexPath.row]

        switch completionMode {
        case .plistFiles(let completion):
            navigationController?.popViewController() {
                completion(appliedFileURL)
            }
        case .imageBestShot(let bestShotCompletion):
            self.bestShotCompletion = bestShotCompletion
            guard let image = UIImage(contentsOfFile: appliedFileURL.path) else {
                presentModalError("errors.invalid_file_format".localized())
                return
            }

            DispatchQueue.main.async { [weak self] in
                self?.bestShotDetector.pushFrame(with: image)
            }
        }
    }

    //  MARK: - CompletionMode -

    enum CompletionMode {
        case plistFiles(completion: (URL) -> Void)
        case imageBestShot((Result<LCBestShot, Error>) -> Void)
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
}

