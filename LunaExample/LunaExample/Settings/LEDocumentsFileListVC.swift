//
//  LEDocumentsFileListVC.swift
//  LunaExample
//
//  Created by Kirill Zhadaev on 12.04.2024.
//

import UIKit
import LunaCamera

final class LEDocumentsFileListVC: UIViewController, UITableViewDelegate, UITableViewDataSource {
    private let VerticalOffset: CGFloat = 10
    private let SideOffset: CGFloat = 10
    private let ApplyButtonHeight: CGFloat = 44

    private var fileURLs: [URL] = []
    private let completion: (URL) -> Void
    private let pathExtension: String?

    private let tableView = UITableView(frame: .zero, style: .plain)

    init(pathExtension: String?, completion: @escaping (URL) -> Void) {
        self.pathExtension = pathExtension
        self.completion = completion
        super.init(nibName: nil, bundle: nil)
    }

    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    override func loadView() {
        super.loadView()

        fileURLs = fetchFileURLs()
        createLayout()
    }

    override func viewWillDisappear(_ animated: Bool) {
        navigationController?.navigationBar.isHidden = true
    }

    override func viewWillAppear(_ animated: Bool) {
        navigationController?.navigationBar.isHidden = false
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
        completion(appliedFileURL)
        navigationController?.popViewController(animated: true)
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

            if let pathExtension {
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

