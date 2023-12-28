//
// Created by IVAN CHIRKOV on 21.04.2021.
//

import Foundation
import LunaCore
import LunaWeb

extension LCBestShot {

    func bestShotData(configuration: LCLunaConfiguration, isWarped: Bool) -> LunaWeb.BestShotData? {
        let jpegData = self.getUIImageWarped(isWarped).jpegData(compressionQuality: CGFloat(configuration.compressionQuality))!
        let data: BestShotData
        switch configuration.bestShotConfiguration.livenessType {
        case .byPhoto:
            data = BestShotData.image(jpegData)
        default:
            let descriptorExtractor = LCDescriptorExtractorBuilder.build(with: configuration)
            let descriptor = descriptorExtractor.extractDescriptorData(from: self)
            data = BestShotData.descriptor(descriptor)
        }
        return data
    }

}
