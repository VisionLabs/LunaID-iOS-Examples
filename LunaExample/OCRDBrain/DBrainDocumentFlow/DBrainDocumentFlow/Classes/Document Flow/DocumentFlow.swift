//
//  DocumentFlow.swift
//  dbraion
//
//  Created by Александрк Бельковский on 03/06/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import Foundation
import UIKit

public class DocumentFlow {
    
    public static let recognitionUrl = URL(string: "https://latest.dbrain.io/recognize?mode=default&hitl_sla=night&quality=75&dpi=300&pdf_raw_images=true")!
    public static let classificationUrl = URL(string: "https://latest.dbrain.io/classify?mode=default&quality=75&dpi=300&pdf_raw_images=true")!
    
    public enum DocumentType {
        case empty
        case custom(type: String)
        case driverLicence
        case passport
        case selectable
    }
    
    public var viewController: UIViewController?
    
    let type: DocumentType
    var lumaDiffCoefficient: CGFloat = 1.0
    var shouldShowDebugElements: Bool = false
    var trackingRect: CGRect!
    
    let authorizationToken: String
    let classificationUrl: URL
    let recognitionUrl: URL
    let fileKey: String
    var expectedSizeKb: Int = 400
    var displayResult: Bool = false
    
    var onEndFlow: (([RecognitionItem]) -> Void)?
    var onReciveResult: ((_ key: String) -> String?)?
    var onReciveDocumentType: ((_ type: String) -> (title: String, isEnabled: Bool))?

    init(type: DocumentType, authorizationToken: String, classificationUrl: URL, recognitionUrl: URL, fileKey: String) {
        self.type = type
        self.authorizationToken = authorizationToken
        self.classificationUrl = classificationUrl
        self.recognitionUrl = recognitionUrl
        self.fileKey = fileKey
        self.trackingRect = defaultTrackingRect()
    }
    
    func defaultTrackingRect() -> CGRect {
        let width = UIScreen.main.bounds.width - 50.0 * 2.0
        let height = width
            
        let size = CGSize(width: width, height: height)
        let origin = CGPoint(x: 50.0, y: 88.0)
            
        return CGRect(origin: origin, size: size)
    }
    
    public static func configure(type: DocumentType, authorizationToken: String, classificationUrl: URL = DocumentFlow.classificationUrl, recognitionUrl: URL = DocumentFlow.recognitionUrl, fileKey: String = "image") -> DocumentFlow {
        let flow = DocumentFlow(type: type, authorizationToken: authorizationToken, classificationUrl: classificationUrl, recognitionUrl: recognitionUrl, fileKey: fileKey)
        
        return flow
    }
    
    public func with(onEndFlow handler: @escaping ([RecognitionItem]) -> Void) -> DocumentFlow {
        onEndFlow = handler
        
        return self
    }
    
    public func with(trackingRect: CGRect) -> DocumentFlow {
        self.trackingRect = trackingRect
        
        return self
    }
    
    public func with(onReciveResult handler: @escaping (_ key: String) -> String?) -> DocumentFlow {
        onReciveResult = handler
        
        return self
    }
    
    public func with(onReciveDocumentType handler: @escaping ((_ type: String) -> (title: String, isEnabled: Bool))) -> DocumentFlow {
        onReciveDocumentType = handler
        
        return self
    }
    
    public func with(expectedSizeKb size: Int) -> DocumentFlow {
        expectedSizeKb = size
        
        return self
    }
    
    public func with(lumaDiffCoefficient coefficient: CGFloat) -> DocumentFlow {
        lumaDiffCoefficient = coefficient
        
        return self
    }
    
    public func withDebugViews() -> DocumentFlow {
        shouldShowDebugElements = true
        
        return self
    }
    
    public func withResult() -> DocumentFlow {
        displayResult = true
        
        return self
    }
    
    public func build() -> DocumentFlow {
        let documentViewController = DocumentViewController()
        documentViewController.type = type
        documentViewController.trackingRect = trackingRect
        documentViewController.lumaDiffCoefficient = lumaDiffCoefficient
        documentViewController.onEndFlow = onEndFlow
        documentViewController.shouldShowDebugElements = shouldShowDebugElements
        documentViewController.authorizationToken = authorizationToken//"UQYkXD0yc72ZV72AoDNOecZBKq2z6LmMJhHQotDhNHMXP6RdN1HQZovqpy7WGye8"
        documentViewController.classificationUrl = classificationUrl//URL(string: "https://latest.dbrain.io/classify?mode=default&quality=75&dpi=300&pdf_raw_images=true")!
        documentViewController.recognitionUrl = recognitionUrl
        documentViewController.fileKey = fileKey//"image"
        documentViewController.expectedSizeKb = expectedSizeKb
        documentViewController.onReciveResult = onReciveResult
        documentViewController.onReciveDocumentType = onReciveDocumentType
        documentViewController.displayResult = displayResult
        
        viewController = documentViewController
        
        return self
    }
    
    public func start() -> UIViewController {
        guard let viewController = viewController else {
            preconditionFailure("Call flow.build() before start")
        }
        
        return viewController
    }
    
}
