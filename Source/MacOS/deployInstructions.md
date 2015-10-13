Deploy Instructions
===================

- Download the project source
- Import the project into [Qt Creator](https://www.qt.io/download/)
- Build it
- Go into the .app package inside the build directory and replace the _Contents/Info.plist_ file with the _Info.plist_ file from this directory

Release Instructions
===================

If you want to create a dmg file for easy deployment on Mac OS out of the _RemoteControlServer.app_, follow these steps:

- Open a terminal at the build directory and run (_Applications/Qt/5.5/clang_64/bin/macdeployqt_)
```
macdeployqt RemoteControlServer.app -verbose=1
```
- If you have code signing certificates, also run
```
codesign --deep --force --verify --verbose --sign "Developer ID Application: Stephan Schultz" RemoteControlServer.app
codesign --verify --verbose=4 RemoteControlServer.app
```
- Copy the _RemoteControlServerTemplate.dmg_ file and drag the new, signed _RemoteControlServer.app_ into it
- Compress the _RemoteControlServerTemplate.dmg_ and commit


Detailed instructions:
[Deploy Qt applications for Mas OS X](http://dragly.org/2012/01/13/deploy-qt-applications-for-mac-os-x/)
