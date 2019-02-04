// @TODO: sockets. Ajax Sux

// Globals
const deviceURL = (typeof overrideDeviceURL !== "undefined") ? overrideDeviceURL : "/";
const setColorUpdateInterval = 100;
var _initialised = false;
var colorPicker = undefined;
var originalColorPickerColor = undefined;
var config = {};

// Scrape the HTML for elements
const $nightLightToggle = $('#switch_nightlight');
const $alarmArmedToggle = $('#switch_alarm_armed');
const $nightlightColorIndicator = $('#indicator_night_light_color');
const $nightlightColorPickerCancelButton = $('#button_set_nightlight_color_cancel');
const $nightlightColorPickerSaveButton = $('#button_set_nightlight_color_save');
const $restartButton = $('#button_restart');
const $restartButtonCancel = $('#button_restart_cancel');
const $restartButtonConfirm = $('#button_restart_confirm');
const $forgetWiFiButton = $('#button_forget_wifi');
const $forgetWiFiButtonCancel = $('#button_forget_wifi_cancel');
const $forgetWiFiButtonConfirm = $('#button_forget_Wifi_confirm');



/**
 * Simple mechanism to know when we were able to connect to the device
 * Pretty lazy, but it works until Sockets is built in.
 */
initialised = function() {
    if (!_initialised){
        _initialised = true;
        showModal('modal_connecting', false);
    }
}


/**
 * Bind all of the events required to drive the control panel
 */
bindEvents = function() {
    // Nightlight On/Off
    $nightLightToggle.on('change', (event) => {
        let newState = $(event.currentTarget).is(":checked");
        setNightLightOn(newState);
    });

    // Nightlight Color Selection
    $nightlightColorIndicator.on('click', () => {
        originalColorPickerColor = colorPicker.getCurColorHex();
        showModal('modal_set_nightlight_color', true);
    })
    $nightlightColorPickerCancelButton.on('click', () => {
        colorPicker.setColorByHex(originalColorPickerColor);
        showModal('modal_set_nightlight_color', false);
    });
    $nightlightColorPickerSaveButton.on('click', () => {
        showModal('modal_set_nightlight_color', false);
    });
    

    // Restart Button
    $restartButton.on('click', () => {
        showModal('modal_restart', true);
    });
    $restartButtonCancel.on('click', () => {
        showModal('modal_restart', false);
    });
    $restartButtonConfirm.on('click', () => {
        restart();
    });

    // Forget Wifi Button
    $forgetWiFiButton.on('click', () => {
        showModal('modal_forget_wifi', true);
    });
    $forgetWiFiButtonCancel.on('click', () => {
        showModal('modal_forget_wifi', false);
    });
    $forgetWiFiButtonConfirm.on('click', () => {
        forgetWiFi();
    });

}



/**
 * Initialise the Color Picker
 */
initColorPicker = function() {
    colorPicker = new KellyColorPicker({
        place : 'picker_night_light_color',
        size : 300,
        popupClass : 'color-picker-popup'
    });

    var setColorTimeout;

    postNewColor = function() {
        clearTimeout(setColorTimeout);
        setColorTimeout = undefined;

        var rgb = colorPicker.getCurColorRgb();
        setNightLightRGB(rgb.r, rgb.g, rgb.b);
    }

    beginSetColorTimeout = function() {
        if (setColorTimeout == undefined) {
            setColorTimeout = setTimeout(() => postNewColor(), setColorUpdateInterval);
        }
    }

    colorPicker.addUserEvent("change", beginSetColorTimeout);
}



/**
 * Resize the color picker to fit the window
 */
autoSizeColorPicker = function() {
    //todo:
    // colorPicker.getWheel().width += 6;
    // colorPicker.getSvFigCursor().radius += 2;
    // colorPicker.getWheelCursor().height += 2;

    // colorPicker.resize(colorPicker.getCanvas().height + 100);
}



/**
 * Ask the device about the state of the controllable items
 * (nightlight state, color, alarm armed state etc...)
 */
queryStates = function() {
    let route = deviceURL + 'config';
    
    let onSuccess = (response) => {
        config = response.config;
        updateUI();
        initialised();
    };

    let onError = () => {
        // Keep trying until we establish a connection
        setTimeout(() => queryStates(), 500);
    }

    ajaxPost(route, undefined, 'GET', undefined, undefined, onSuccess, onError);
}



/**
 * Set the night light on or off
 * @var {bool} on true or false to set the night light on or off
 */
setNightLightOn = function(on) {
    let route = deviceURL + 'setnightlighton';
    let data = {
        "on": on
    };

    let onSuccess = () => {
        // console.log('yep');
    }
    
    //@TODO: Error handling / screen waiting
    ajaxPost(route, data, 'GET', undefined, undefined, onSuccess);
}



/**
 * Set the night light on or off
 * @var {number} r the red value of the nightlight
 * @var {number} g the green value of the nightlight
 * @var {number} b the blue value of the nightlight
 */
setNightLightRGB = function(r, g, b) {
    let route = deviceURL + 'setnightlightrgb';
    let data = {
        "r": r,
        "g": g,
        "b": b
    };

    let onSuccess = () => {
        let nightLightColor = rgbToHex(data);
        $nightlightColorIndicator.css('background-color', nightLightColor);
    }
    
    //@TODO: Error handling / screen waiting
    ajaxPost(route, data, 'GET', undefined, undefined, onSuccess);
}



/**
 * Restart the device
 */
restart = function() {
    let route = deviceURL + 'restart';
    let data = {};
    
    let onSuccess = () => {
        _initialised = false;
        showModal('modal_connecting', true);
        setTimeout(() => queryStates(), 1000);
    }

    let onComplete = () => {
        showModal('modal_restart', false);
    }

    //@TODO: Error handling / screen waiting
    ajaxPost(route, data, 'GET', undefined, undefined, onSuccess, undefined, onComplete);
}



/**
 * Forget WiFi
 */
forgetWiFi = function() {
    let route = deviceURL + 'forgetwifi';
    let data = {};
    
    let onSuccess = () => {
        // console.log('yep');
    }

    let onComplete = () => {
        showModal('modal_forget_wifi', false);
    }

    //@TODO: Error handling / screen waiting
    ajaxPost(route, data, 'GET', undefined, undefined, onSuccess, undefined, onComplete);
}



/**
 * The Hallway Monitor control panel uses MiniCSS which utilises checkboxes for showing modals.
 * This code progamattically checks / unchecks those checkboxes to show the modals.
 */
showModal = function(modalId, state) {
    $('#' + modalId).prop('checked', state);
}



/**
 * Update the UI to match the current state
 */
updateUI = function() {
    $nightLightToggle.prop("checked", config.nightlightOn);
    $alarmArmedToggle.prop("checked", config.sensorEnabled);
    let nightLightColor = rgbToHex({r: config.r, g: config.g, b: config.b});
    colorPicker.setColorByHex(nightLightColor);
    $nightlightColorIndicator.css('background-color', nightLightColor);
}



/** 
 * Convert a set of RGB colors to HEX
 * @param {object} color provide color.r, color.g and color.b to convert to hex
 */
function rgbToHex(color) {
    var componentToHex = function (c) {
        var hex = c.toString(16);
        return hex.length === 1 ? "0" + hex : hex;
    };

    return "#" + componentToHex(color.r) + componentToHex(color.g) + componentToHex(color.b);
}



/**
 * Fired when the document is loaded and ready
 */
$( document ).ready(function() {
    initColorPicker();
    bindEvents();
    queryStates();
});