# LED Controller for K2 Bar
This app is composed of an electron app and an arduino file. The arduino controls LEDs via a custom DMX protocol and the electron app lets the user control the color and brightness of the LEDs. This application is specifically built for K2 Bar.

# Arduino Controller
Using a modified version of DmxSimple:
https://github.com/PaulStoffregen/DmxSimple

# Electron App
Use yarn instead of npm

To use electron-rebuild, setup your Visual Studio version via 
```
yarn run node-gyp configure --msvs_version=2017
```
or
```
npm config edit
```
