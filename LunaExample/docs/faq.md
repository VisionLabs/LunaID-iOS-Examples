# LUNA ID iOS FAQ

Some common problems

# Where I can find bestshot?

You can find bestshot in LMCameraDelegate:
```swift
    func bestShot(_ bestShot: LCBestShot) {
        let bestShotImage: UIImage = bestShot.getUIImageWarped(false)
    }
```
 
 # How I can avoid liveness check?

 If you want to make only local checks without involving LunaPlatform, you can set LivenessType with .none
 ```swift
    private lazy var configuration: LCLunaConfiguration = {
        let configuration = LCLunaConfiguration.defaultConfig()
        ...
        configuration.bestShotConfiguration.livenessType = .none
        ...
        return configuration
    }()

 ```
