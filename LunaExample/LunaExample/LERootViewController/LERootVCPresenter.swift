//
//  LERootVCPresenter.swift
//  LunaExample
//
//  Created by  Геращенко Андрей on 26.09.2024.
//

import LunaWeb
import LunaCore

class LERootVCPresenter {
    
    typealias FacesResultHandler = (Result<String, Error>) -> Void
    
    enum FacesQueryError: Error {
        case lifecycleTimeout
    }
    
    let configuration: LCLunaConfiguration
    let webconfiguration: LWConfig
    private lazy var lunaAPI = LunaWeb.APIv6(lunaAccountID: webconfiguration.lunaAccountID,
                                             lunaServerURL: webconfiguration.platformURL) { [weak self] _ in
                     guard let platformToken = self?.webconfiguration.platformToken
                        else { return [:] }
                     return [APIv6Constants.Headers.authorization.rawValue: platformToken]
                 }
    
    init(configuration: LCLunaConfiguration, webconfiguration: LWConfig) {
        self.configuration = configuration
        self.webconfiguration = webconfiguration
    }
    
    func fetchFace(for userName: String,
                   completion: @escaping FacesResultHandler) {
        let query = GetFacesQuery(userData: userName, targets: [.userData, .faceID])
        lunaAPI.faces.getFaces(query: query) { result in
            DispatchQueue.main.async { [weak self] in
                guard let self else {
                    completion(.failure(FacesQueryError.lifecycleTimeout))
                    return
                }
                switch result {
                case .success(let response):
                    if let faceID = response.faces.first(where: { $0.userData == userName })?.id {
                        completion(.success(faceID))
                    } else {
                        completion(.failure(LEServerError.noUser))
                    }
                case .failure(let error):
                    completion(.failure(error))
                }
            }
        }
    }
}
