// @TODO: sockets. Ajax Sux

// Globals
const deviceURL = "http://hallwaymonitor/";

// Scrape the HTML for elements
const $nightLightToggle = $('#switch_nightlight');
const $restartButton = $('#button_restart');
const $restartButtonCancel = $('#button_restart_cancel');
const $restartButtonConfirm = $('#button_restart_confirm');
const $forgetWiFiButton = $('#button_forget_wifi');
const $forgetWiFiButtonCancel = $('#button_forget_wifi_cancel');
const $forgetWiFiButtonConfirm = $('#button_forget_Wifi_confirm');


/**
 * Bind all of the events required to drive the control panel
 */
bindEvents = function() {
    var self = this;

    // Nightlight On/Off
    $nightLightToggle.on('change', function() {
        let newState = $(this).is(":checked");
        self.setNightLight(newState);
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
 * Ask the device about the state of the controllable items
 * (nightlight state, color, alarm armed state etc...)
 */
queryStates = function() {
    //@TODO:
}



/**
 * Set the night light on or off
 * @var {bool} on true or false to set the night light on or off
 */
setNightLight = function(on) {
    let route = deviceURL + 'setnightlighton';
    let data = {
        "on": on
    };

    let onSuccess = () => {
        console.log('yep');
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
        console.log('yep');
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
        console.log('yep');
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
    bindEvents();
    queryStates();
});