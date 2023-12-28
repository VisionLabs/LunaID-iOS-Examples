//
//  DocumentLoader.swift
//  dbraion
//
//  Created by Александрк Бельковский on 04/06/2019.
//  Copyright © 2019 Александрк Бельковский. All rights reserved.
//

import Foundation
import UIKit

enum DocumentLoaderError: Error {
    case serverError
    case invalidParse
}

struct ClassificationResponse: Decodable {
    var items: [ClassificationItem]
    var code: Int?
    var message: String?
    var errno: Int?
}

struct ClassificationItem: Decodable {
    var document: ClassificationDocument
    var crop: String
}

struct ClassificationDocument: Decodable {
    var title: String?
    var isEnabled: Bool?
    
    var type: String
    var rotation: Int
    var coords: [[Int]]
}

struct RecognitionResponse: Decodable {
    var items: [RecognitionItem]
    var code: Int?
    var message: String?
    var errno: Int?
}

public struct RecognitionItem: Decodable {
    public var docType: String
    public var fields: [String: RecognitionField]
}

public struct RecognitionField: Decodable {
    public var text: String
    public var confidence: Double
}

class DocumentLoader: NSObject {
    
    private let classificationUrl: URL
    private let recognitionUrl: URL
    private let authorizationToken: String
    private let fileKey: String
    private let expectedSizeKb: Int
    
    private let queue = DispatchQueue(label: "com.document.loader.queue")
    
    init(classificationUrl: URL, recognitionUrl: URL, authorizationToken: String, fileKey: String, expectedSizeKb: Int) {
        self.classificationUrl = classificationUrl
        self.recognitionUrl = recognitionUrl
        self.authorizationToken = authorizationToken
        self.fileKey = fileKey
        self.expectedSizeKb = expectedSizeKb
        
        super.init()
    }
    
    func recognition(image: UIImage, type: String?, completion: @escaping (Result<RecognitionResponse, Error>) -> Void) {
        queue.async {
            guard let imageData = image.compressTo(expectedSizeKb: self.expectedSizeKb) else {
                return
            }
            
            guard var components = URLComponents(url: self.recognitionUrl, resolvingAgainstBaseURL: true) else {
                return
            }
            
            var queryItems = components.queryItems ?? []
            if let type = type {
                queryItems.append(URLQueryItem(name: "doc_type", value: type))
            }
            components.queryItems = queryItems
            
            guard let url = components.url else {
                return
            }
            
            var request = URLRequest(url: url)
            
            request.httpMethod = "POST"
            
            let boundary = self.generateBoundaryString()
            request.setValue("application/json", forHTTPHeaderField: "Accept")
            request.setValue(self.authorizationToken, forHTTPHeaderField: "Authorization")
            request.setValue("multipart/form-data; charset=utf-8; boundary=\(boundary)", forHTTPHeaderField: "Content-Type")
            request.httpBody = self.bodyWithParameters(parameters: nil,
                                                       mimetype: "image/jpeg",
                                                       filePathKey: self.fileKey,
                                                       filename: "\(NSUUID().uuidString).jpeg",
                fileData: imageData,
                boundary: boundary)
            
            let configuration = URLSessionConfiguration.default
            let session = URLSession(configuration: configuration, delegate: self, delegateQueue: OperationQueue.main)
            
            
            let task = session.dataTask(with: request) { data, _, error -> Void in
                session.finishTasksAndInvalidate()
                
                guard let data = data else {
                    DispatchQueue.main.async {
                        if let error = error {
                            completion(.failure(error))
                        } else {
                            completion(.failure(DocumentLoaderError.serverError))
                        }
                    }
                    return
                }
                
                let decoder = JSONDecoder()
                decoder.keyDecodingStrategy = .convertFromSnakeCase
                
                do {
                    let r = try decoder.decode(RecognitionResponse.self, from: data)
                    DispatchQueue.main.async {
                        completion(.success(r))
                    }
                } catch {
                    DispatchQueue.main.async {
                        completion(.failure(error))
                    }
                }
                
            }
            
            task.resume()
            
        }
    }
    
    func classification(image: UIImage, completion: @escaping (Result<ClassificationResponse, Error>) -> Void) {
        queue.async { [weak self] in
            guard let self = self else {
                return
            }
            
            guard let imageData = image.compressTo(expectedSizeKb: self.expectedSizeKb) else {
                return
            }
            
            var request = URLRequest(url: self.classificationUrl)
            
            request.httpMethod = "POST"
            
            let boundary = self.generateBoundaryString()
            request.setValue("application/json", forHTTPHeaderField: "Accept")
            request.setValue(self.authorizationToken, forHTTPHeaderField: "Authorization")
            request.setValue("multipart/form-data; charset=utf-8; boundary=\(boundary)", forHTTPHeaderField: "Content-Type")
            request.httpBody = self.bodyWithParameters(parameters: nil,
                                                       mimetype: "image/jpeg",
                                                       filePathKey: self.fileKey,
                                                       filename: "\(NSUUID().uuidString).jpeg",
                                                       fileData: imageData,
                                                       boundary: boundary)
            
            let configuration = URLSessionConfiguration.default
            let session = URLSession(configuration: configuration, delegate: self, delegateQueue: OperationQueue.main)
            
            
            let task = session.dataTask(with: request) { data, _, error -> Void in
                session.finishTasksAndInvalidate()
                
                guard let data = data else {
                    DispatchQueue.main.async {
                        if let error = error {
                            completion(.failure(error))
                        } else {
                            completion(.failure(DocumentLoaderError.serverError))
                        }
                    }

                    return
                }
                
                let decoder = JSONDecoder()
                decoder.keyDecodingStrategy = .convertFromSnakeCase
                
                do {
                    let r = try decoder.decode(ClassificationResponse.self, from: data)
                    DispatchQueue.main.async {
                        completion(.success(r))
                    }
                } catch {
                    DispatchQueue.main.async {
                        completion(.failure(error))
                    }
                }
                
            }
            
            task.resume()
         
        }
    }
    
    private func generateBoundaryString() -> String {
        return "Boundary-\(NSUUID().uuidString)"
    }
    
    private func bodyWithParameters(parameters: [String: String]?, mimetype: String,
                                    filePathKey: String, filename: String,
                                    fileData: Data, boundary: String) -> Data {
        var body = Data()
        
        if let parameters = parameters {
            for (key, value) in parameters {
                body.appendString("--\(boundary)\r\n")
                body.appendString("Content-Disposition: form-data; name=\"\(key)\"\r\n\r\n")
                body.appendString("\(value)\r\n")
            }
        }
        
        body.appendString("--\(boundary)\r\n")
        body.appendString("Content-Disposition: form-data; name=\"\(filePathKey)\"; filename=\"\(filename)\"\r\n")
        body.appendString("Content-Type: \(mimetype)\r\n\r\n")
        body.append(fileData)
        body.appendString("\r\n")
        body.appendString("--\(boundary)--\r\n")
        
        return body
    }
}

// MARK: – URLSession delegate
extension DocumentLoader: URLSessionDelegate {
    
    // Disable TLS
    func urlSession(_ session: URLSession, didReceive challenge: URLAuthenticationChallenge, completionHandler: @escaping (URLSession.AuthChallengeDisposition, URLCredential?) -> Void) {
        var credential: URLCredential?
        
        if let serverTrust = challenge.protectionSpace.serverTrust {
            credential = URLCredential(trust: serverTrust)
        }
        
        completionHandler(.useCredential, credential)
    }
    
}
