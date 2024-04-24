//
//  DocumentDropDownViewController.swift
//  dbraion
//
//  Created by Александрк Бельковский on 10/06/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import UIKit

class DocumentDropDownViewController: UIViewController {
    
    private var values: [String]
    private var selectedIndex: Int
    var type: String = ""
    
    var onDidSelect: ((Int) -> Void)?
    
    private var mainView: DocumentDropDownView {
        return view as! DocumentDropDownView
    }
    
    // MARK: - Lifecycle
    
    init(values: [String], selectedIndex: Int) {
        self.values = values
        self.selectedIndex = selectedIndex
        
        super.init(nibName: nil, bundle: nil)
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    override func loadView() {
        view = DocumentDropDownView()
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        mainView.tableView.register(DocumentDropDownCellView.self, forCellReuseIdentifier: "documentDropDownCellView")
        
        mainView.tableView.delegate = self
        mainView.tableView.dataSource = self
    }
    
}

// MARK: - UITableViewDelegate, UITableViewDataSource
extension DocumentDropDownViewController: UITableViewDelegate, UITableViewDataSource {
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return values.count
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "documentDropDownCellView", for: indexPath) as! DocumentDropDownCellView
        
        cell.titleLabel.text = values[indexPath.row]
        
        if indexPath.row == selectedIndex {
            cell.titleLabel.textColor = UIColor(red: 0.0 / 255.0, green: 116.0 / 255.0, blue: 243.0 / 255.0, alpha: 1.0)
            cell.iconShapeLayer.opacity = 1.0
        } else {
            cell.titleLabel.textColor = UIColor(red: 25.0 / 255.0, green: 28.0 / 255.0, blue: 31.0 / 255.0, alpha: 1.0)
            cell.iconShapeLayer.opacity = 0.0
        }
        
        return cell
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        onDidSelect?(indexPath.row)
    }
    
    func tableView(_ tableView: UITableView, heightForRowAt indexPath: IndexPath) -> CGFloat {
        return DocumentDropDownCellView.cellHeight()
    }
    
}
