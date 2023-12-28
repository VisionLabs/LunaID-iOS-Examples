//
//  SupportedDocumentTypes.swift
//  SESmartIDSample
//
//  Created by Artuhay on 17.07.2020.
//  Copyright © 2020 biz.smartengines. All rights reserved.
//

import Foundation

extension SmartIDViewControllerSwift {
	@objc func getDocumentTypeList() -> [String:[String]] {
		let supportDocTypes = self.supportedDocTypes()
    
    var dictionary = [String:[String]]()
    for docTypes in supportDocTypes {
      print(docTypes)
      for dl in docTypes {
        let splitedDl = dl.split(separator: ".")
				if splitedDl.count > 1 {
					let mask = dl.dropLast(splitedDl.last!.count) + "*"
					if dictionary[String(splitedDl[0])] == nil {
						dictionary[String(splitedDl[0])] = [String]()
					}
					if var arr = dictionary[String(splitedDl[0])] {
						if String((splitedDl.last)!).hasPrefix("type") {
						arr.append(String(mask))
						} else {
							arr.append(dl)
						}
						dictionary[String(splitedDl[0])] = arr
					}
				} else {
					if dictionary[String(splitedDl[0])] == nil {
						dictionary[String(splitedDl[0])] = [String]()
					}
					if var arr = dictionary[String(splitedDl[0])] {
						arr.append(String(splitedDl[0]))
						dictionary[String(splitedDl[0])] = arr
					}
				}
      }
    }
		
    return dictionary
  }
}
