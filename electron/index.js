const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
const storage = require('electron-json-storage');
const iro = require('@jaames/iro');
const $ = require('jquery');
require('bootstrap');
require('popper.js');

const parser = new Readline();
var port = new SerialPort(".", { autoOpen: false });

var colors = [];
var leds = [];
var colorPickers = {};
var nextColorKey = 1;


window.onerror = (errorMsg, url, lineNumber) => {
    document.getElementById("error-modal-text").innerText = `
        Ein unbekannter Fehler ist aufgetreten. Bitte starten Sie das Programm neu!
        Sollte dies keine Wirkung zeigen, suchen Sie bitte den nächstgelegenen Computer-Menschen
        auf oder schreien Sie laut um Hilfe. Hier die Fehlermeldung:\n
        ${errorMsg}\n
        Zeile ${lineNumber} in folgender Datei:\n
        ${url}
    `;
    $("#error-modal").modal("show");
    return false;
}


async function init() {
    // get all devices connected
    const ports = await SerialPort.list();
    arduinos = ports.filter((el) => {
        return el.hasOwnProperty('manufacturer') && el.manufacturer.startsWith('Arduino LLC')
    });
    if(arduinos.length === 0) {
        document.getElementById("error-modal-text").innerText = `
            Es wurde kein Arduino zum Steuern der LEDs gefunden. Bitte schließen Sie ein Gerät an und starten Sie das Programm neu.
        `.replace(/\n/g, "");
        $("#error-modal").modal("show");
    } else if(arduinos.length > 1) {
        document.getElementById("error-modal-text").innerText = `
            Es wurden ${arduinos.length} Arduinos zum Steuern der LEDs gefunden. Bitte entfernen Sie ein oder mehrere Geräte und starten Sie das Programm neu.
        `.replace(/\n/g, "");
        $("#error-modal").modal("show");
    } else {
        // if only one device is connected, enable communication
        port = new SerialPort(arduinos[0].path, { baudRate: 9600 });
        port.pipe(parser);
        parser.on('data', line => {
            console.log(`> ${line}`);
            if(line.toString().startsWith("Greetings!")) {
                updateLedIconsAndOutput();
            }
        });
        port.on('open', () => {
            document.getElementById("port-alert").innerText = `
                Verbunden mit ${arduinos[0].path}
            `.replace(/\n/g, "");
            $("#port-alert").fadeTo(3000, 500);
        });
    }

    // read colors from storage and add html elements for each
    storage.get("colors", (error, data) => {
        if (error)
            throw error;

        // check if data has expected format
        if(Object.entries(data).length > 0) {
            colors = data;
        } else {
            // if no key is found, data defaults to {}, so length === 0
            onAddColorTile();
            storage.set("colors", colors);
        }

        // get index that was never used to build key
        colors.forEach(el => {
            nextColorKey = Math.max(nextColorKey, el.key + 1);
        });

        // insert html elements
        colors.forEach(colorObj => insertColorHtmlElement(colorObj));

        // now colors should be loaded, then load led colors
        // get keys of colors assigned to leds
        // 6 RGB + 5 White -> 11 LEDs
        storage.get("leds", (error, data) => {
            if (error)
                throw error;
            
            // check if data has expected format
            if(Object.entries(data).length === 11) {
                leds = data;
            } else {
                // if no key is found, data defaults to {}
                if(colors.length === 0)
                    onAddColorTile();
                leds = Array(11).fill(colors[0].key);
                storage.set("leds", leds);
            }

            // leds array will now have length 11
            // do 11 iterations:
            //      insert led tile for each iteration
            //      for 7th iteration: insert empty tile before
            var isRgb = Array(6).fill(true).concat(Array(5).fill(false));
            var ledTitles = ["Theke", "Tür", "Kicker", "Ecke 1", "Wand", "Ecke 2", "Tür Weiß", "Kicker Weiß", "Ecke 1 Weiß", "Wand Weiß", "Ecke 2 Weiß"]
            var container = document.getElementById("led-tiles");

            for (let index = 0; index < 11; index++) {
                if(index === 6)
                    container.insertAdjacentHTML("beforeend", `<div class="empty-tile led-tile"></div>`);

                var colorObj = colors.find(item => {
                    return item.key == leds[index];
                });

                if(isRgb[index])
                    var colorHexString = new iro.Color(colorObj.color).hexString;
                else 
                    var colorHexString = new iro.Color({h: 0, s: 0, v: colorObj.color.v}).hexString;

                var elementHtml = `
                    <div id="led-tile-${index}" class="led-tile">
                        <div class="led-name-container">
                            <div class="led-name-icon">
                                <i class="centered material-icons-outlined" style="color: ${colorHexString}">wb_sunny</i>
                            </div>
                            <div class="led-name">${ledTitles[index]}</div>
                        </div>
                        <div id="dropdown-${index}" class="dropdown">
                            <button class="btn btn-secondary dropdown-toggle" type="button" id="dropdownMenuButton" data-toggle="dropdown" aria-haspopup="true" aria-expanded="true" data-boundary="viewport">
                                ${colorObj.name}
                            </button>
                            <div class="dropdown-menu" aria-labelledby="dropdownMenuButton"></div>
                        </div>
                    </div>
                `;
                container.insertAdjacentHTML("beforeend", elementHtml);
            }
            updateLedDropdownMenus();
        });
    });
}

function insertColorHtmlElement(colorObj) {
    var container = document.getElementById("color-tiles");
    var iroColor = new iro.Color(colorObj.color);
    var specialAddTile = container.lastChild.previousSibling;
    container.removeChild(specialAddTile);

    // build color tile element
    var elementHtml = `
        <div id="color-tile-${colorObj.key}" class="color-tile">
            <div
                class="color-display"
                style="background-color: ${iroColor.hexString};"
                onclick="onDeleteColor(this)"
                onmouseover="onMouseOverIcon(this, false)" 
                onmouseout="onMouseOutIcon(this, false)">
                <i class="centered material-icons">delete_forever</i>
            </div>
            <div class="color-name" ondblclick="onChangeColorName(this)">${colorObj.name}</div>
            <div class="wheel" id="color-wheel-${colorObj.key}"></div>
            <div class="color-name" ondblclick="onChangeColorHex(this)">${iroColor.hexString.toUpperCase()}</div>
        </div>
    `;
    container.insertAdjacentHTML("beforeend", elementHtml);

    var colorWheel = iro.ColorPicker("#color-wheel-" + colorObj.key, {
        width: 150,
        color: colorObj.color,
        padding: 0,
        handleRadius: 8,
        wheelLightness: false,
    });
    
    colorWheel.on('input:change', color => {
        colorObj.color = color.hsv;
        var tileNode = document.getElementById("color-tile-" + colorObj.key);

        // set background color of color display
        tileNode.firstElementChild.style.backgroundColor = color.hexString;
        // set inner text of hex display to hex string
        tileNode.lastElementChild.innerText = color.hexString.toUpperCase();

        updateLedIconsAndOutput();
    })
    
    colorWheel.on('input:end', color => {
        colorObj.color = color.hsv;
        storage.set("colors", colors);
        updateLedDropdownMenus();
    })

    colorPickers[colorObj.key] = colorWheel;

    // re-insert the blank tile for adding new colors
    container.insertBefore(specialAddTile, container.lastChild);
}




function updateLedDropdownMenus() {
    for(let parent of document.getElementsByClassName("dropdown-menu")) {
        // remove all dropdown menu options
        var child = parent.lastElementChild;  
        while (child) {
            parent.removeChild(child); 
            child = parent.lastElementChild;
        }

        // insert new dropdown menu options
        colors.forEach(colorObj => {
            var elementHtml = `
                <button class="dropdown-item" onclick="onSelectLedColor(this, ${colorObj.key})">
                    <span>${colorObj.name}</span>
                    <div class="dropdown-color-circle" style="background-color: ${new iro.Color(colorObj.color).hexString};"></div>
                </button>
            `;
            parent.insertAdjacentHTML("beforeend", elementHtml);
        });
    }
}

function updateLedIconsAndOutput() {
    // display non-rgb leds as grayscale
    var isRgb = Array(6).fill(true).concat(Array(5).fill(false));
    var outputData = Array(28).fill(0);
    var channelIndexRgb = 4;
    var channelIndexWhite = 11;

    for (let index = 0; index < leds.length; index++) {
        const colorObj = colors.find(item => {
            return item.key === leds[index];
        });
        if(isRgb[index])
            var iroColor = new iro.Color(colorObj.color);
        else
            var iroColor = new iro.Color({h: 0, s: 0, v: colorObj.color.v});

        // set color of icon
        var ledTile = document.getElementById("led-tile-" + index);
        var icon = ledTile.firstElementChild.firstElementChild.firstElementChild;
        icon.style = "color: " + iroColor.hexString;

        // set data to output to serial port
        if(isRgb[index]) {
            const rgb = iroColor.rgb;
            if(index === 0) {
                // channels r and b of first rgb led are switched
                outputData[channelIndexRgb] = rgb.b;
                outputData[channelIndexRgb + 1] = rgb.g;
                outputData[channelIndexRgb + 2] = rgb.r;
            } else {
                outputData[channelIndexRgb] = rgb.r;
                outputData[channelIndexRgb + 1] = rgb.g;
                outputData[channelIndexRgb + 2] = rgb.b;
            }
            channelIndexRgb += 4;
        } else {
            outputData[channelIndexWhite] = Math.floor((iroColor.hsv.v / 100) * 255);
            channelIndexWhite += 4;
        }
    }
    
    // write data to serial port
    port.write(outputData.join(",") + "\n", error => {
        if(error)
            throw error;
    });
}




function onSelectLedColor(element, colorKey) {
    var color = colors.find(item => {
        return item.key === colorKey;
    });
    var dropdownNode = element.parentNode.parentNode;
    var ledIndex = parseInt(dropdownNode.id.substring("dropdown-".length));
    leds[ledIndex] = colorKey;
    dropdownNode.firstElementChild.innerText = color.name;
    storage.set("leds", leds);
    updateLedIconsAndOutput();
}

function onAddColorTile() {
    // color format: obj with hsv values
    colors.push({
        "key": nextColorKey,
        "name": "Farbe " + nextColorKey,
        "color": {h: 1, s: 1, v: 1}
    });
    nextColorKey++;
    storage.set("colors", colors);
    insertColorHtmlElement(colors[colors.length - 1]);
    updateLedDropdownMenus();
}

function onDeleteColor(element) {
    const tile = element.parentNode;
    const colorObj = colors.find(item => {
        return ("color-tile-" + item.key) === tile.id;
    });

    // check if color is still used
    for(let ledKey of leds)
        if(ledKey === colorObj.key) {
            $("#color-deleted-modal").modal("show");
            return;
        }
    
    // remove color picker from dict
    delete colorPickers[colorObj.key];

    // remove color from dataset
    this.colors = colors.filter(item => {
        return ("color-tile-" + item.key) !== tile.id;
    });
    tile.parentNode.removeChild(tile);

    storage.set("colors", colors);
    updateLedDropdownMenus();
}

function onChangeColorName(element) {
    var parent = element.parentNode;
    var itemIndex = colors.findIndex(item => {
        return ("color-tile-" + item.key) === parent.id;
    });
    var textInput = document.createElement("INPUT");
    textInput.setAttribute("type", "text");
    textInput.setAttribute("value", element.innerText);
    textInput.addEventListener("keyup", event => {
        if (event.keyCode === 13) {
            event.preventDefault();

            if(textInput.value !== "") {
                colors[itemIndex].name = textInput.value;
                storage.set("colors", colors);
            }

            // replace text input with div
            var nameDiv = document.createElement("div");
            nameDiv.setAttribute("class", "color-name");
            nameDiv.setAttribute("ondblclick", "onChangeColorName(this)");
            nameDiv.innerText = colors[itemIndex].name;
            parent.replaceChild(nameDiv, textInput);

            // for each led in leds -> if color is selected, update dropdown button text
            for (let index in leds) {
                if(leds[index] !== colors[itemIndex].key)
                    continue;
                const element = document.getElementById("dropdown-" + index);
                element.firstElementChild.innerText = colors[itemIndex].name;
            }
            updateLedDropdownMenus();
        }
    });
    parent.replaceChild(textInput, element);
    textInput.select();
}

function onChangeColorHex(element) {
    var colorKey = element.parentNode.id.substring("color-tile-".length);
    var button = document.getElementById("hex-modal-submit");
    var input = document.getElementById("hex-modal-input");

    $("#hex-modal-form").submit((event) => {
        event.preventDefault();
        var colorObj = colors.find(item => {
            return item.key == colorKey;
        });

        var hexValue = "#" + input.value;
        // test for valid syntax
        if(!/^#([0-9A-F]{3}){1,2}$/i.test(hexValue)) {
            $('#hex-modal-feedback').show();
            $('#hex-modal-input').addClass("is-invalid");
            return;
        }
        
        var iroColor = new iro.Color(hexValue);
        colorObj.color = iroColor.hsv;

        colorPicker = colorPickers[colorObj.key];
        colorPicker.color.set(colorObj.color);
        colorPicker.emit("input:change", iroColor);
        colorPicker.emit("input:end", iroColor);
        $('#hex-input-modal').modal('hide');
        $('#hex-modal-feedback').hide();
        $('#hex-modal-input').removeClass("is-invalid");
        input.value = "";
    });
    
    $('#hex-input-modal').modal('show');
}


function onMouseOverIcon(element, setParentBorder) {
    // set color of icon and border
    var color = getComputedStyle(document.body).getPropertyValue('--light-grey');
    element.firstElementChild.style.color = color;
    if(setParentBorder)
        element.style.borderColor = color;
}

function onMouseOutIcon(element, setParentBorder) {
    // set color of icon and border
    var color = getComputedStyle(document.body).getPropertyValue('--dark-blue');
    element.firstElementChild.style.color = color;
    if(setParentBorder)
        element.style.borderColor = color;
}