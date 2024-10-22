//
// Created by IVAN CHIRKOV on 21.04.2021.
//

import Foundation
import LunaCore
import LunaWeb

extension LCBestShot {

    func bestShotData(configuration: LCLunaConfiguration, isWarped: Bool) -> LunaWeb.BestShotData? {
        guard let jpegData = self.getUIImageWarped(isWarped).jpegData(compressionQuality: CGFloat(configuration.compressionQuality)) else { return nil }
        return BestShotData.image(jpegData)
    }

}
