//
//  GeneralDocumentView.swift
//  dbraion
//
//  Created by Александрк Бельковский on 08/06/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import UIKit

class GeneralDocumentView: BaseDocumentView {
    
    var cameraTextZoneView: CameraTextZoneView = {
        let view = CameraTextZoneView()
        view.translatesAutoresizingMaskIntoConstraints = false
        view.isUserInteractionEnabled = false
        
        return view
    }()
    
    private let trackingRect: CGRect
    
    // MARK: - Lifecycle
    
    override init(trackingRect: CGRect) {
        self.trackingRect = trackingRect
        
        super.init(trackingRect: trackingRect)
        
        cameraPreviewView.insertSubview(cameraTextZoneView, at: 1)
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    // MARK: - Layout
    
    override func layoutSubviews() {
        super.layoutSubviews()
        
        cameraTextZoneView.frame = trackingRect
    }
    
    override func view(apply: Bool) {
        super.view(apply: apply)
        cameraTextZoneView.textZone(apply: apply)
    }
    
}

