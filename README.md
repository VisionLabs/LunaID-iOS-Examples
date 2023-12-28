# LunaID-iOS-Examples

In this repository you can find 3 example applications on how to integrate LunaID SDK under iOS platform:
- Liveness Demo - the simplest case of SDK usage when you launch camera view controller and then check the results.
- LunaExample - uses 3 scenarios or flows which are incapsulated into LunaAuth framework: register, identify and verify.
- LunaExampleOCR - is the most advanced case because it is not only show how to work with 3 kind of flows but adds OCR check in it.

LunaID SDK uses UIKit and Swift under cover. Minimal supported iOS version is 13. You can use latest XCode version to develop your application. Bitcode support enabled for any compatability you may need.

These examples are not include LunaID SDK itself. You need to request the latest release from your project manager. From the provided SDK you need to copy "frameworks" folder directly into example folder. For example in case of Liveness Demo it should be like this:
```
Liveness Demo/frameworks/flower.xcframework
Liveness Demo/frameworks/fsdk.xcframework
Liveness Demo/frameworks/tsdk.xcframework
Liveness Demo/frameworks/LunaAuth.xcframework
Liveness Demo/frameworks/LunaCamera.xcframework
Liveness Demo/frameworks/LunaCore.xcframework
Liveness Demo/frameworks/LunaWeb.xcframework
