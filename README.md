### About

This application is an example of how you can use LunaID. In here you can see 3 scenarios:

- identification of a user
- verification of a user
- registration

every of these scenarios can be proceeded with OCR

### **Swift Package Manager**

**INSTALLATION**

To get SPM working you need to get access to repository with provided by VisionLabs credentials. After that you should define these credentials in file ~/.netrc of your home directory. Here is the correct working example of this file

```bash
machine ##########
login ##########
password ##########
````

next give this file proper access rights with this command

```bash
% chmod 600 ~/.netrc
```

And also you need to add you github account into XCode settings. After that SPM should download LunaID itself and all the dependencies it needs. It's very important to **select 'Exact Version'** when adding LunaID package.

**UPDATE**

Due to some SPM peculiarities, if you want to update LunaID package to a new version, we **strongly recommend** deleting package from your Package dependencies, and then adding it again. It's very important to **select 'Exact Version'** when adding package via SPM.

### License

To launch these examples you need *.plist license file provided by VisionLabs
