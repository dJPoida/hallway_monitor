// @TODO: sockets. Ajax Sux

// Globals
const deviceURL = (typeof overrideDeviceURL !== "undefined") ? overrideDeviceURL : "/";
const setColorUpdateInterval = 100;
var colorPicker = undefined;

// Scrape the HTML for elements
const $nightLightToggle = $('#switch_nightlight');
const $restartButton = $('#button_restart');
const $restartButtonCancel = $('#button_restart_cancel');
const $restartButtonConfirm = $('#button_restart_confirm');
const $forgetWiFiButton = $('#button_forget_wifi');
const $forgetWiFiButtonCancel = $('#button_forget_wifi_cancel');
const $forgetWiFiButtonConfirm = $('#button_forget_Wifi_confirm');
const $nightLightColorInput = $('#input_color')


/**
 * Bind all of the events required to drive the control panel
 */
bindEvents = function() {
    var self = this;

    // Nightlight On/Off
    $nightLightToggle.on('change', function() {
        let newState = $(this).is(":checked");
        self.setNightLightOn(newState);
    });

    // Restart Button
    $restartButton.on('click', function() {
        showModal('modal_restart', true);
    });
    $restartButtonCancel.on('click', function() {
        showModal('modal_restart', false);
    });
    $restartButtonConfirm.on('click', function() {
        restart();
    });

    // Forget Wifi Button
    $forgetWiFiButton.on('click', function() {
        showModal('modal_forget_wifi', true);
    });
    $forgetWiFiButtonCancel.on('click', function() {
        showModal('modal_forget_wifi', false);
    });
    $forgetWiFiButtonConfirm.on('click', function() {
        forgetWiFi();
    });

}



/**
 * Initialise the Color Picker
 */
initColorPicker = function() {
    colorPicker = new KellyColorPicker({
        input : 'input_color', 
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
    //@TODO:

    //TODO: use this to set the color picker current value
    //colorPicker.setColorByHex('#3b8db1');
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
        // console.log('yep');
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
        // console.log('yep');
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
 * Fired when the document is loaded and ready
 */
$( document ).ready(function() {
    initColorPicker();
    bindEvents();
    queryStates();
});