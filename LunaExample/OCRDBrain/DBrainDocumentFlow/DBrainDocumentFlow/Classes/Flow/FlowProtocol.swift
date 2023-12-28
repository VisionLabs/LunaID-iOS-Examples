//
//  FlowProtocol.swift
//  dbraion
//
//  Created by Александрк Бельковский on 03/06/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import Foundation
import UIKit

public protocol FlowProtocol {
    
    var viewController: UIViewController? { get }
    
    var onEndFlow: (() -> Void)? { get }
    
    var authorizationToken: String { get }
    var uploadUrl: URL { get }
    var fileKey: String { get }
    
    func with(onEndFlow handler: @escaping () -> Void) -> Self
    func with(uploadUrl url: URL) -> Self
    func with(authorizationToken token: String) -> Self
    func with(fileKey key: String) -> Self
    func with(expectedSizeKb size: Int) -> Self
    func build() -> Self
    func start() -> UIViewController
}
