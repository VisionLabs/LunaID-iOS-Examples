To start using LUNA ID for iOS examples, you need to activate the license.

To activate the license:

1. Request **Server**, **EID**, and **ProductID** from VisionLabs. For details, see [License parameters](#license-parameters).

2. Specify the received parameters in the "vllicense.plist" file and save the changes.

3. Add the file to your final app.

The license key will be generated and saved to the specified directory. The license file has a binary format. At the next launch of the mobile app on the same device, the license will be read from this file.

You can optionally rename the "vllicense.plist" file. To do this, change the default value, which is `vllicense.plist`, of the `LCLunaConfiguration::plistLicenseFileName` property.

#### Example license file

Below is a sample content of the "vllicense.plist" file:

```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>Server</key>
    <string></string>
    <key>ProductID</key>
    <string></string>
    <key>EID</key>
    <string></string>
    <key>ContainerMode</key>
    <real></real>
    <key>ConnectionTimeout</key>
    <integer></integer>
    <key>Filename</key>
    <string>license.dat</string>
</dict>
</plist>
```

#### License parameters

License parameters and further processing requires the following parameter:

| Parameter           | Description | Type | Default value | Required |
| ----------          | ----------- | ---- | ------------  | -------- |
| `Server`            | Activation server URL.         | String | Not set | Yes |
| `EID`               | Entitlement ID. | String | Not set | Yes |
| `ProductID`         | Product ID. | String | Not set | Yes |
| `Filename`          | The default name of the file to save the license to after activation. The maximum length of the file name is 64 symbols. We do not recommend that you change this name. | String | Not set | No |
| `ContainerMode`     | If run in container. | Real | 0 | No |
| `ConnectionTimeout` | The maximum time, in seconds, for the transfer operation to take. Setting the timeout to 0 means that it never times out during transfer. You can't set the parameter to a negative value. The maximum value is 300 seconds. | Integer | 15 | No |