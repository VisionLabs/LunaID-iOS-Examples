//
// Created by IVAN CHIRKOV on 21.04.2021.
//

import Foundation

extension String {

    func localized() -> String {
        return NSLocalizedString(self, tableName: nil, bundle: Bundle(for: OCRService.self), value: "", comment: "")
    }

}
