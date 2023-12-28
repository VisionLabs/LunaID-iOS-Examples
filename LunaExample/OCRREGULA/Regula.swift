//
//  Regula.swift
//  OCRREGULA
//
//  Created by IVAN CHIRKOV on 10.03.2021.
//

import Foundation
import DocumentReader

public class Regula {

    private init() {}
    public static let shared = Regula()
    
    public func initialize(licenseData: Data, completion: @escaping (Result<Void, Swift.Error>) -> Void) {        
        prepareDB { (result) in
            switch result {
            case .success:
                DocReader.shared.initializeReader(config: DocReader.Config(license: licenseData)) { (success, error) in
                    if success {
                        completion(.success(()))
                    } else if let error = error {
                        completion(.failure(error))
                    } else {
                        completion(.failure(Error.unknown))
                    }
                }

            case .failure(let error):
                completion(.failure(error))
            }
        }
    }
    
    public func showScanner(on viewController: UIViewController, completion: @escaping (Result<DocumentReaderResults, Swift.Error>?) -> Void) {
        DocReader.shared.processParams.scenario = RGL_SCENARIO_OCR
        DocReader.shared.processParams.doublePageSpread = true

        DocReader.shared.showScanner(viewController) { (action, result, error) in
            switch action {
            case .complete:
                if let result = result {
                    completion(.success(result))
                }
            case .processTimeout:
                completion(nil)
            case .cancel:
                completion(nil)
            case .error:
                completion(.failure(error!))
            default:
                break
            }
        }
    }
    
    private func prepareDB(completion: @escaping (Result<Void, Swift.Error>) -> Void) {
        DocReader.shared.prepareDatabase(databaseID: "Full") { (progress) in
            #if DEBUG
            let progressValue = String(format: "%.1f", progress.fractionCompleted * 100)
            print("PREPARE DB:", progressValue)
            #endif
        } completion: { (success, error) in
            if success {
                completion(.success(()))
            } else {
                completion(.failure(error!))
            }
        }
    }
    
}
