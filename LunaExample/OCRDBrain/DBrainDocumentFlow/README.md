## Contents

- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Flare](#flare)
- [Credits](#credits)
- [License](#license)

## Requirements

- iOS 9.0+
- Xcode 10.0+
- Swift 4.0+

## Installation

### CocoaPods

[CocoaPods](http://cocoapods.org) is a dependency manager for Cocoa projects. You can install it with the following command:

```bash
$ gem install cocoapods
```

To integrate DBrainDocumentFlow into your Xcode project using CocoaPods, specify it in your `Podfile`:

```ruby
source 'https://github.com/CocoaPods/Specs.git'
platform :ios, '9.0'
use_frameworks!

target '<Your Target Name>' do
    pod 'DBrainDocumentFlow'
end
```

Then, run the following command:

```bash
$ pod install
```
---
## Usage

### Flow

- `DocumentFlow` - Base flow. When take photo you can choose type of document, before send to recognize;
- `PassportFlow` - inherited flow from DocumentFlow. By default recognize passport_main type;
- `DriverLicenceFlow` - inherited flow from DocumentFlow. By default recognize driver_license_2011_front type.

### Types

`DocumentType` - Possible document types are specified when creating` DocumentFlow`

- `.empty` - Does not specify doc_type when sending for recognition
- `custom (type: String)` - Specifies a custom doc_type when submitting
- `driverLicence` - Does not specify doc_type when sending for recognition
- `passport` - Specifies passport_main doc_type when sending
- `selectable` - After the photo, a choice will be offered if the classification was successful

### Methods

- Handler end flow, used to close the window and get data after recognition (call when photo upload success), default `nil`
#### Swift
```swift
let onEndFlow: ([RecognitionItem]) -> Void = { [weak self] data in
    print(data)
    self?.dismiss(animated: true)
}

flow.with(onEndFlow: onEndFlow)
```

- `RecognitionItem`
The object of the field obtained in the recognition result

#### Swift
```swift
struct RecognitionItem: Decodable {
     var docType: String // Document type
     var fields: [String: RecognitionField] // Document fields
}

struct RecognitionField: Decodable {
     var text: String // Field value
     var confidence: Double // Accuracy
}
```

- Maximum photo size in kilobytes, default `400`
#### Swift
```swift
flow.with(expectedSizeKb: 400)
```

- Setting the tracking area, by default the area is calculated in the same way as in the example
#### Swift
```swift
let width = UIScreen.main.bounds.width - 50.0 * 2.0
let height = width * 1.3

let size = CGSize(width: width, height: height)
let origin = CGPoint(x: 50.0, y: 88.0)

let zoneRect = CGRect(origin: origin, size: size)

flow.with(trackingRect: zoneRect)
```

- Maximum flare level, default is `1` (flare is not taken into account), details [Flare](#flare)
#### Swift
```swift
let lumaDiffCoefficient: CGFloat = 0.35

flow.with(lumaDiffCoefficient: lumaDiffCoefficient)
```

- Display of histogram and maximum illumination level, `disabled` by default
#### Swift
```swift
flow.withDebugViews()
```

- Displaying the result upon successful recognition, `disabled` by default
#### Swift
```swift
flow.withResult()
```

- The headler of the result obtained as a result of the analysis, allows you to return strings for display, by default `nil` (Also, if you return` nil`, the information will not be displayed)
#### Swift
```swift
let keyTitles: [String: String] = [
    "date_of_birth": "Date of birth: ",
    "date_of_issue": "Date of issue: ",
    "first_name": "First name: ",
    "issuing_authority": "Issuing authority: ",
    "other_names": "Other names: ",
    "place_of_birth": "Place of birth: ",
    "series_and_number": "Series and number: ",
    "sex": "Sex: ",
    "subdivision_code": "Subdivision code: ",
    "surname": "Surname: "
]

let onReciveResult: ((_ key: String) -> String?) = { key in
    return keyTitles[key]
}

documentFlow.with(onReciveResult: onReciveResult)
```

- Headler of the result obtained as a result of classification, allows to return strings and make available / unavailable recognition, by default `nil`
#### Swift
```swift
let onReciveDocumentType: ((_ type: String) -> (title: String, isEnabled: Bool)) = { type in
    return (type.replacingOccurrences(of: "_", with: " ").capitalized, type != "not_document")
}

documentFlow.with(onReciveDocumentType: onReciveDocumentType)
```

- Module creation and module display. Before creation, you need to call the `build ()` method, after which, using the `start ()` method, you can get the configured `UIViewController`
#### Swift
```swift
// Some configration
flow = flow.build()
let viewController = flow.start()
present(viewController, animated: true)
```

### Example

- Create DocumentFlow
#### Swift
```swift
let onEndFlow: () -> Void = { [weak self] in
    self?.dismiss(animated: true)
}

let keyTitles: [String: String] = [
    "some_key": "Some key"
]

let onReciveResult: ((_ key: String) -> String?) = { key in
    return keyTitles[key] ?? key
}

let onReciveDocumentType: ((_ type: String) -> (title: String, isEnabled: Bool)) = { type in
    return (type.replacingOccurrences(of: "_", with: " ").capitalized, type != "not_document")
}

let side = UIScreen.main.bounds.width - 50.0 * 2.0
let size = CGSize(width: side, height: side)
let origin = CGPoint(x: 50.0, y: 88.0)

let flow = DocumentFlow.configure(authorizationToken: token)
    .with(onEndFlow: onEndFlow)
    .with(onReciveResult: onReciveResult)
    .with(onReciveDocumentType: onReciveDocumentType)
    .with(lumaDiffCoefficient: 0.70)
    .with(expectedSizeKb: 1000)
    .with(trackingRect: CGRect(origin: origin, size: size))
    .withDebugViews()
    .build()

let viewController = flow.start()
viewController.modalPresentationStyle = .fullScreen

present(viewController, animated: true)
```

- Create PassportFlow
#### Swift
```swift
let flow = PassportFlow.configure(authorizationToken: token)
    .with(onEndFlow: onEndFlow)
    .build()

let viewController = flow.start()
viewController.modalPresentationStyle = .fullScreen

present(viewController, animated: true)
```

## Flare
When creating `DocumentFlow`, `DriverLicenceFlow`  or `PassportFlow` , you can specify `lumaDiffCoefficient` from` 0` to `1`, if the value is greater than` 1`, then `1` will be accepted, i.e. everything will be accepted and light will not be counted.
This factor sets the maximum height of the right edge of the histogram
`Max = maxValue * lumaDiffCoefficient`

![Luma](https://github.com/DeadHipo/DBrainDocumentFlow/blob/main/luma_clipped.png)

[Coefficients for calculating bleed from RGB histogram](https://developer.apple.com/documentation/accelerate/vimage/converting_color_images_to_grayscale)
or
[Here](https://www.japanistry.com/histograms/)

## Credits

- Alex Belkovskiy ([@nearlydeadhipo](https://t.me/nearlydeadhipo))

## License

DBrainDocumentFlow is released under the MIT license. See LICENSE for details.
