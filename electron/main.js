const { app, BrowserWindow } = require('electron');
const path = require('path');
const url = require('url');

let mainWindow;

function createWindow() {
  mainWindow = new BrowserWindow({
    width: 1500,
    height: 800,
    webPreferences: {
      // preload: path.join(__dirname, 'preload.js'),
      nodeIntegration: true
    }
  })

  mainWindow.loadURL(url.format({
      pathname: path.join(__dirname, 'frequency_test/mm_test.html'),
      // pathname: path.join(__dirname, 'index.html'),
      protocol: 'file:',
      slashes: true
  }))

  mainWindow.on('closed', function () {
    mainWindow = null
  })
  mainWindow.webContents.openDevTools()
}

app.on('ready', createWindow)

app.on('window-all-closed', function () {
  if (process.platform !== 'darwin')
    app.quit();
})

app.on('activate', function () {
  if (mainWindow === null)
    createWindow()
})

// hide context security policy warning
process.env['ELECTRON_DISABLE_SECURITY_WARNINGS'] = 'true';