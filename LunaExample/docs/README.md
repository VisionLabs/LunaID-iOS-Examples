# LUNA ID iOS SDK

This SDK is adopted to be usefull in swift world of iOS development. Here are some examples of SDK integration https://github.com/VisionLabs/LunaID-iOS-Examples 

# Release notes LUNA ID iOS 1.9.0

- bug fixing

# Release notes LUNA ID iOS 1.8.6

- fixing memory leak while frame processing

# Release notes LUNA ID iOS 1.8.5

- fixing memory leak while frame processing

# Release notes LUNA ID iOS 1.8.4

- fixing multithreaded bug in terms of face identity feature

# Release notes LUNA ID iOS 1.8.3

- fixing LCLunaConfiguration.trackFaceIdentity
- fixing interactions delay

# Release notes LUNA ID iOS 1.8.2

- Decreased quality of saved video to 180x320
- Fixed bug with interactions timeout setup

# Release notes LUNA ID iOS 1.8.1

- LCLunaConfiguration.glassesCheckEnabled added to define if you want or not to track if the person wearing a glasses

# Release notes LUNA ID iOS 1.8.0

- LCLunaConfiguration.trackFaceIdentity added to define if you want or not to track indetity of face during video session

# Release notes LUNA ID iOS 1.7.9

- adding delay between interactions
- every interaction sends notification about it's end/start/inprogress

### How to define timeout between interactions

You can use LCLunaConfiguration.interactionsConfig.timeoutBetweenInteractions property to define delay between interactions(in seconds).

You can find notifications about interaction status from userInfo[NSStepStateKey] in NSError object which you will receive in bestshotError() delegate method. E.g.
```swift        
    func bestShotError(_ error: Error) {
        if ((error as NSError).code == BestShotError.NEED_TO_BLINK.rawValue) {
            print("blink interaction state <\((error as NSError).userInfo[NSStepStateKey] ?? 0)>")
        }
    }
```

# Release notes LUNA ID iOS 1.7.8

- fixing aspect ration for low resolution video file

# Release notes LUNA ID iOS 1.7.7

- video file size reduced for iOS15 and lower

# Release notes LUNA ID iOS 1.7.6

- stabilization fixes

# Release notes LUNA ID iOS 1.7.5

- stabilization fixes

# Release notes LUNA ID iOS 1.7.4

- stabilization fixes

# Release notes LUNA ID iOS 1.7.3

- fixes for face time feature

# Release notes LUNA ID iOS 1.7.2

- licensing now can be defined in terms of final application
- you have timer to check that your face is remains in focus for any time period

### Face focus time

It is possible to define time period in which alarm will be triggered in case of face disappearing. You can define LCLunaConfiguration::faceTime property in seconds to define watchdog logic for face disappearing. In case the face disappeared you can catch error BestShotError.FACE_LOST in delegate LCBestShotDelegate::bestShotError

### Licensing

Please pay attention that since this moment you need to define your license in your final application. NOT in data/license.conf. data/license.conf should remain empty!
Ok, here is the vllicense.plist file you need to add to you final application. It should be in Budle of your application!
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>Server</key>
    <string>https://visionlabsbv.prod.sentinelcloud.com</string>
    <key>ProductID</key>
    <string>b67e61f3-24f4-47e1-bef8-d73dca4a4c2d</string>
    <key>EID</key>
    <string>ae889e72-2729-4973-91b4-b6201715e960</string>
    <key>ContainerMode</key>
    <real>0</real>
    <key>ConnectionTimeout</key>
    <integer>15</integer>
    <key>Filename</key>
    <string>license.dat</string>
</dict>
</plist>
```

If it is necessary you can redefine default file name of license property list. You can find LCLunaConfiguration::plistLicenseFileName property with default value vllicense.plist and change it to whatever you want.

# Release notes LUNA ID iOS 1.7.1

- Now you can manualy setup interactions
- occluded face bug fix

### Manual interactions sequence

You can define your own sequence of interactions using LMCameraViewControllerProtocol::defineInteractionsStep method, e.g.:
```obj-c
            let cameraViewController = LMCameraBuilder.viewController(delegate: self,
                                                                      configuration: self.configuration,
                                                                      livenessAPI: self.livenessAPI)
            cameraViewController.defineInteractionsStep([
                LunaCore.LCBlinkConfig(),
                LunaCore.LCDownHeadTrackConfig(),
                LunaCore.LCUpHeadTrackConfig()
            ])
            cameraViewController.dismissHandler = { [weak self] in
                self?.closeViewController(animated: true)
            }
            cameraViewController.modalPresentationStyle = .fullScreen
            self.present(cameraViewController, animated: true)
```

As you can see you can define array of LCStepConfigProtocol objects. Now there are only 5 such objects:
- LCBlinkConfig
- LCUpHeadTrackConfig
- LCDownHeadTrackConfig
- LCLeftHeadTrackConfig
- LCRightHeadTrackConfig

LCBlinkConfig gives you a chance to control blinking interactions. It is timeout only.

Head tracking configs give you a chance to define not only timeout but a range of angles in which head tracking should be made

# Release notes LUNA ID iOS 1.7.0

- implemented timeout in seconds before the face recognition begins
- added recognition the state if the face is occluded
- now you can check liveness localy without requests to Luna Platform
- blinking recognition now works even for one-eyed people

You can setup delay in seconds to define when the reocgnition should be started after the camera is showed on screen. Please use LCLunaConfiguration.startDelay

If you want to define the situation when the face is occluded with somekind object please setup LCLunaConfiguration.occludeCheck = true

To use liveness check in offline mode first please make sure that you have fsdk.framework/data/oslm_v4_model_1_arm.plan and fsdk.framework/data/oslm_v4_model_2_arm.plan files in your deploy. Then setup LCLunaConfiguration.useOfflineLiveness = true

# Release notes LUNA ID iOS 1.6.1

- Universal simulator binary for XCode15 and lower XCode versions

# Release notes LUNA ID iOS 1.6.0

- update engine to version 5.16
- tracking face all the time the camera is on screen
- adding config for interactions

### Using config to controll interactions

LCLunaConfiguration class now contains property 

```obj-c
@property (nonatomic, strong) LCInteractionsConfig *interactionsConfig;
```

This property give you the ability to:
- setup timeout of every separate interaction using interactionTimeout
- setup number of interactions, which should not exceed 5 using stepsNumber.

### Switching to another plan

If you would like to use another plan file. By default to calculate descriptors it uses plan file version 59. To switch to plan file with version 52 you need to modify config file fsdk.framework/data/faceengine.conf, simply switch value 59
```xml
<param name="model" type="Value::Int1" x="59" />
```

to value 52

```xml
<param name="model" type="Value::Int1" x="52" />

```

# Release notes LUNA ID iOS 1.5.0

- Added custom interactions sequence

### Custom interactions sequence

You have interaction generator which will generate random sequence of interactions. Now it will be only 2 interactions which were selected in random order from this list:
- asking to move your head right
- asking to move your head left
- asking to move your head up
- asking to move your head down
- asking to blink

# Release notes LUNA ID iOS 1.4.1

- Implemented alternative way to save videos for iOS15<

# Release notes LUNA ID iOS 1.4.0

- Fixing bugs in blink functionality
- Record video only in case face was detected
- Provide error structure for notifications
- Full control of show/hide camera screen

### Record face detected feeds

If you want to record only those feeds where faces were detected then you should set to true LCLunaConfiguration->saveOnlyFaceVideo  

### Show/hide control

From now on if you want to show or hide LMCameraViewControllerProtocol you should implement it by your self. LMCameraViewControllerProtocol has property dismissHandler. So for example:

```swift
        let controller = LMCameraBuilder.viewController(delegate: self,
                                                        configuration: configuration,
                                                        livenessAPI: livenessAPI,
                                                        recordVideo: false)
        controller.dismissHandler = { [weak self] in
            guard let self = self else {
                return
            }
            self.dismiss(animated: true)
        }
        present(controller, animated: true)
```

# Release notes LUNA ID iOS 1.3.1
- Fix LivenessType cases

# Release notes LUNA ID iOS 1.3.0
- Video recording
- Support of custom HTTP-headers
- Support simulators

### Custom HTTP headers

To add custom header you can use LunaWeb.LivenessAPIv6(configuration:, additionalHeaders:) where the second parameter is a closure which will be launched every time any request is going to send. You can add any logic you want in here

### Video record

To enable recording of video stream you need to define recordVideo parameter as true in 

```swift
        let controller = LMCameraBuilder.viewController(delegate: self,
                                                          configuration: configuration,
                                                          livenessAPI: livenessAPI,
                                                        recordVideo: true)
```

To find the resulting video file you need to implement method     

```swift
func videoRecordFinished(videoFile: String)
```

in your LMCameraDelegate

# Release notes LUNA ID iOS 1.2.0

- SDK weight optimization 
- Multiple faces error processing
- Fixed crash in case of pre-builder usage
- License renewal update

In case you are not going to use descriptor calculation on application side(it means that all the photos you are sending to server) you can decrease SDK size even more by removing files:

- fsdk.framework/data/cnn59m_arm.plan
- fsdk.framework/data/cnndescriptor_59.conf

### Luna SDK integration

First you need to do is to get your license EID and ProductID and type it in into fsdk.framework/data/license.conf file
Next you need is to define your IDs and LunaPlatform url in AppDelegate or any other place but before any attempts to use engine. Should be something like this:

```swift
        let configuration = LCLunaConfiguration.defaultConfig()
        configuration.identifyHandlerID = "xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx"
        configuration.registrationHandlerID = "xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx"
        configuration.verifyID = "xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx"
        configuration.lunaAccountID = "xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx"
        configuration.lunaServerURL = URL(string: "https://luna-platform.visionlabs.ru/6")
```

### Luna SDK customization

Currently there are two customization points:
- you can define LMCustomization object in call of LMCameraBuilder::viewController() method
- you can define LMCustomization object in calls of LunaFlowBuilder class

The main customization object is LMCustomization class. So you need to create an object of LMCustomization. It contains uiCustomizer property of LMUICustomizerProtocol protocol. You can create your own implementation of LMUICustomizerProtocol methods which will return your views implementation.

LMUICustomizerProtocol supports customization of two things:
- the tracking-frame which tracks face position on video feed from camera
- notifications view which are appearing on top of the video screen feed

To customize tracking frame you need to implement UIView which supports LMFaceDetectionViewProtocol. In this protocol there are 2 methods you need to support and which are trigered by LunaID SDK:
 - switchToPositiveState - when tracking process goes well
 - switchToNegativeState - when something is wrong with the face on video feed

Please keep in mind that LunaID SDK can modify LMFaceDetectionViewProtocol frame at any time. It affects size and position of view.

To customize notifications view on top of video feed screen you need to implement UIView which supports LMVideoStreamNotificationViewProtocol. There are 2 method you need to implememnt:
- showNotificationMessage - this methods is trigerred by LunaID SDK in case you need to show notification message
- notificationMessage - this method should return current notification message

The default implementation of LMVideoStreamNotificationViewProtocol is LMVideoStreamNotificationView class. You can create an object of LMVideoStreamNotificationView class and customize font or text color with applyFont() and applyTextColor() methods accordingly.

### Localizing notification messages

Currently you can localize any notifications which will be displayed at top of LMCameraViewControllerProtocol using customization approach. LMVideoStreamNotificationViewProtocol has method

```swift
    func showNotificationMessage(_ newMessage: String)
```

You can implement you own variant of notification view with LMVideoStreamNotificationViewProtocol and replace all incoming messages with you own localization.
