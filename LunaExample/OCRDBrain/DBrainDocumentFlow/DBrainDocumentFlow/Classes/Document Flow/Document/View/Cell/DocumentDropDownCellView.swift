//
//  DocumentDropDownCellView.swift
//  dbraion
//
//  Created by Александрк Бельковский on 10/06/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import UIKit

class DocumentDropDownCellView: UITableViewCell {
    
    private var previousBounds: CGRect = .zero
    
    lazy var iconShapeLayer: CAShapeLayer = {
        let layer = CAShapeLayer()
        layer.fillColor = UIColor.clear.cgColor
        layer.strokeColor = UIColor(red: 0.0 / 255.0, green: 117.0 / 255.0, blue: 235.0 / 255.0, alpha: 1.0).cgColor
        layer.lineWidth = 2.0
        layer.lineCap = .round
        
        self.layer.addSublayer(layer)
        
        return layer
    }()
    
    let titleLabel: UILabel = {
        let label = UILabel()
        label.translatesAutoresizingMaskIntoConstraints = false
        label.textColor = UIColor(red: 25.0 / 255.0, green: 28.0 / 255.0, blue: 31.0 / 255.0, alpha: 1.0)
        label.textAlignment = .left
        label.font = UIFont.systemFont(ofSize: 16.0, weight: .regular)
        
        return label
    }()
    
    // MARK: - Lifecycle
    
    override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
        super.init(style: style, reuseIdentifier: reuseIdentifier)
        contentView.backgroundColor = .white
        contentView.addSubview(titleLabel)
        
        setupConstraints()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    override func prepareForReuse() {
        super.prepareForReuse()
        
        titleLabel.textColor = UIColor(red: 25.0 / 255.0, green: 28.0 / 255.0, blue: 31.0 / 255.0, alpha: 1.0)
    }
    
    // MARK: - Layout
    
    override func layoutSubviews() {
        super.layoutSubviews()
        
        if previousBounds != bounds {
            updateIconPath()
            previousBounds = bounds
        }
    }
    
    private func updateIconPath() {
        let path = UIBezierPath()
        
        let startX = bounds.maxX - 17.0 - 24.0
        let startY = bounds.midY - 0.5
        let start = CGPoint(x: startX, y: startY)
        
        let middleX = bounds.maxX - 12.0 - 24.0
        let middleY = bounds.midY + 4.5
        let middle = CGPoint(x: middleX, y: middleY)
        
        let endX = bounds.maxX - 2.0 - 24.0
        let endY = bounds.midY - 4.5
        let end = CGPoint(x: endX, y: endY)
        
        path.move(to: start)
        path.addLine(to: middle)
        path.addLine(to: end)
        
        iconShapeLayer.path = path.cgPath
    }
    
    private func setupConstraints() {
        
        titleLabel.centerYAnchor.constraint(equalTo: centerYAnchor).isActive = true
        titleLabel.leadingAnchor.constraint(equalTo: leadingAnchor, constant: 24.0).isActive = true
        
    }
    
    static func cellHeight() -> CGFloat {
        return 56.0
    }
    
}
