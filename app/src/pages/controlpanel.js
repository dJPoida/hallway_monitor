const Globals = require ('../lib/globals');
const KellyColorPicker = require ('../lib/kcp');

// Globals
// const setColorUpdateInterval = 100;

/**
 * @class ControlPanel
 * The main client side code which controls the control panel
 */
module.exports = class ControlPanel {
    
    /**
     * Constructor
     */
    constructor() {
        this._connected = false;

        this._nightLightOn = false;
        this._nightLightRGB = {
            r: 255,
            g: 255,
            b: 255
        };
        this._iftttEnabled = false;
        this._iftttEndpoint = "";
        this._iftttServer = "";
        this._sensorEnabled = false;
        this._colorPicker = undefined;
        this._originalColorPickerColor = undefined;

        this.initialise();
    }



    /**
     * Initialise the Page
     */
    initialise = () => {
        console.log('initialise()');
        this.scrapeHTML();
        this.initColorPicker();
        this.bindEvents();
        this.queryStates();
    }



    /**
     * Bind all of the events required to drive the control panel
     */
    bindEvents = () => {
        console.log('bindEvents()');

        // Nightlight On/Off
        this.$nightLightToggle.on('change', (event) => {
            this.nightLightOn = $(event.currentTarget).is(":checked");
        });

        // Nightlight Color Selection
        this.$nightlightColorIndicator.on('click', () => {
            this._originalnightLightRGB = {r: this.nightLightRGB.r, g: this.nightLightRGB.g, b: this.nightLightRGB.b};
            Globals.showModal('modal_set_nightlight_color', true);
        })
        this.$nightlightColorPickerCancelButton.on('click', () => {
            this.nightLightRGB = this._originalnightLightRGB;
            Globals.showModal('modal_set_nightlight_color', false);
        });
        this.$nightlightColorPickerSaveButton.on('click', () => {
            Globals.showModal('modal_set_nightlight_color', false);
        });

        // Sensor Enabled
        this.$sensorEnabledToggle.on('change', (event) => {
            this.sensorEnabled = $(event.currentTarget).is(":checked");
        });

        // Restart Button
        this.$restartButton.on('click', () => {
            Globals.showModal('modal_restart', true);
        });
        this.$restartButtonCancel.on('click', () => {
            Globals.showModal('modal_restart', false);
        });
        this.$restartButtonConfirm.on('click', () => {
            this.restart();
        });

        // Forget Wifi Button
        this.$forgetWiFiButton.on('click', () => {
            Globals.showModal('modal_forget_wifi', true);
        });
        this.$forgetWiFiButtonCancel.on('click', () => {
            Globals.showModal('modal_forget_wifi', false);
        });
        this.$forgetWiFiButtonConfirm.on('click', () => {
            this.forgetWiFi();
        });
    }



    /**
     * Scrape the HTML for components we'll need as part of the application
     */
    scrapeHTML = () => {
        console.log('scrapeHTML()');
        this.$nightLightToggle = $('#switch_nightlight');
        this.$sensorEnabledToggle = $('#switch_alarm_armed');
        this.$nightlightColorIndicator = $('#indicator_night_light_color');
        this.$nightlightColorPickerCancelButton = $('#button_set_nightlight_color_cancel');
        this.$nightlightColorPickerSaveButton = $('#button_set_nightlight_color_save');
        this.$restartButton = $('#button_restart');
        this.$restartButtonCancel = $('#button_restart_cancel');
        this.$restartButtonConfirm = $('#button_restart_confirm');
        this.$forgetWiFiButton = $('#button_forget_wifi');
        this.$forgetWiFiButtonCancel = $('#button_forget_wifi_cancel');
        this.$forgetWiFiButtonConfirm = $('#button_forget_Wifi_confirm');
    }


    /**
     * Initialise the Color Picker
     */
    initColorPicker = () => {
        console.log('initColorPicker()');
        this._colorPicker = new KellyColorPicker({
            place : 'picker_night_light_color',
            size : 300,
            popupClass : 'color-picker-popup'
        });

        let setColorTimeout;

        postNewColor = () => {
            clearTimeout(setColorTimeout);
            setColorTimeout = undefined;

            this.nightLightRGB = this.colorPicker.getCurColorRgb();
        }

        beginSetColorTimeout = () => {
            if (setColorTimeout == undefined) {
                setColorTimeout = setTimeout(() => postNewColor(), 50);
            }
        }

        this.colorPicker.addUserEvent("change", beginSetColorTimeout);
    }



    /**
     * Ask the device about the state of the controllable items
     * (nightlight state, color, alarm armed state etc...)
     */
    queryStates = () => {
        let route = Globals.deviceURL() + 'config';
        
        let onSuccess = (response) => {
            console.log("queryStates() response: ");
            let config = response.config;
            console.log(config);

            // Set the current known values from the config JSON
            this._nightLightOn = config.nightlightOn;
            this._nightLightRGB = {r: config.r, g: config.g, b: config.b};
            this._iftttEnabled = config.iftttEnabled;
            this._iftttEndpoint = config.iftttEndpoint;
            this._iftttServer = config.iftttServer;
            this._sensorEnabled = config.sensorEnabled;

            this.updateUI();
            this.connected = true;
        };

        let onError = () => {
            // Keep trying until we establish a connection
            setTimeout(() => this.queryStates(), 500);
        }

        Globals.ajaxPost(route, undefined, 'GET', undefined, undefined, onSuccess, onError);
    }



    /**
     * Update the UI to match the current state
     */
    updateUI = () => {
        this.$nightLightToggle.prop("checked", this.nightLightOn);
        this.$sensorEnabledToggle.prop("checked", this.sensorEnabled);
        this.colorPicker.setColorByHex(this.nightLightRGB);
        this.$nightlightColorIndicator.css('background-color', Globals.rgbToHex(this.nightLightRGB));
    }



    /**
     * @property {object} colorPicker the accessor for the Color Picker
     */
    get colorPicker() {return this._colorPicker;}



    /**
     * @property nightLightOn whether the nightlight is on at the moment
     */
    get connected() {return this._connected;}
    set connected(value) {
        Globals.showModal('modal_connecting', !value);
    }



    /**
     * @property nightLightOn whether the nightlight is on at the moment
     */
    get nightLightOn() {return this._nightLightOn;}
    set nightLightOn(value) {
        if (value != this._nightLightOn) {
            let oldValue = this._nightLightOn;
            this._nightLightOn = value;
            
            // Disable the toggle
            this.$nightLightToggle.prop("disabled", true);

            let route = Globals.deviceURL() + 'setnightlighton';
            let data = {
                "on": value
            };

            // What to do when the call is successful
            let onSuccess = () => {
                console.log('Successfully set the nightlight to ' + value);
            }

            // What to do when the call fails
            let onError = () => {
                this._nightLightOn = oldValue;
                this.updateUI();
                // @todo: error? toast?
            }
            
            // What to do in either situation
            let onComplete = () => {
                this.$nightLightToggle.prop("disabled", false);
                this.updateUI();
            }

            Globals.ajaxPost(route, data, 'GET', undefined, undefined, onSuccess, onError, onComplete);
        }
    }



    /**
     * @property {object} nightLightRGB the current r, g, & b values of the nightlight
     */
    get nightLightRGB() {return this._nightLightRGB;}
    set nightLightRGB(value) {
        if (
            (value.r != this._nightLightRGB.r) ||
            (value.g != this._nightLightRGB.g) ||
            (value.b != this._nightLightRGB.b) 
        ) {
            var oldValue = this._nightLightRGB;
            this._nightLightRGB = value;
    
            let route = Globals.deviceURL() + 'setnightlightrgb';
            let data = this._nightLightRGB;

            // What to do when the call is successful
            let onSuccess = () => {
                console.log('Successfully set the nightlight RGB to ' + data);
            }

            // What to do when the call fails
            let onError = () => {
                this._nightLightRGB = oldValue;
                this.updateUI();
                // @todo: error? toast?
            }
            
            // What to do in either situation
            let onComplete = () => {
                this.updateUI();
            }

            Globals.ajaxPost(route, data, 'GET', undefined, undefined, onSuccess, onError, onComplete);
        }

    }



    /**
     * @property sensorEnabled whether the sensor is enabled (armed) at the moment
     */
    get sensorEnabled() {return this._sensorEnabled;}
    set sensorEnabled(value) {
        if (value != this._sensorEnabled) {
            let oldValue = this._sensorEnabled;
            this._sensorEnabled = value;
            
            // Disable the toggle
            this.$sensorEnabledToggle.prop("disabled", true);

            let route = Globals.deviceURL() + 'setsensorenabled';
            let data = {
                "sensorEnabled": value
            };

            // What to do when the call is successful
            let onSuccess = () => {
                console.log('Successfully set sensorEnabled to ' + value);
            }

            // What to do when the call fails
            let onError = () => {
                this._sensorEnabled = oldValue;
                this.updateUI();
                // @todo: error? toast?
            }
            
            // What to do in either situation
            let onComplete = () => {
                this.$sensorEnabledToggle.prop("disabled", false);
                this.updateUI();
            }

            Globals.ajaxPost(route, data, 'GET', undefined, undefined, onSuccess, onError, onComplete);
        }
    }



    /**
     * Restart the device
     */
    restart = function() {
        let route = Globals.deviceURL() + 'restart';
        let data = {};
        
        let onSuccess = () => {
            Globals.showModal('modal_restart', false);
            this.connected = false;
            setTimeout(() => this.queryStates(), 1000);
        }

        let onError = () => {
            //@TODO: Error handling - toast?
        }

        Globals.ajaxPost(route, data, 'GET', undefined, undefined, onSuccess, onError);
    }



    /**
     * Forget the details of the current WiFi network and restart
     */
    forgetWiFi = function() {
        let route = Globals.deviceURL() + 'forgetwifi';
        let data = {};
        
        let onSuccess = () => {
            Globals.showModal('modal_forget_wifi', false);

            Globals.showModal('modal_wifi_forgotten', true);
            this._connected = false;
        }

        let onError = () => {
            //@TODO: Error handling - toast?
        }

        Globals.ajaxPost(route, data, 'GET', undefined, undefined, onSuccess, onError);
    }
}






// /**
//  * Resize the color picker to fit the window
//  */
// autoSizeColorPicker = function() {
//     //todo:
//     // colorPicker.getWheel().width += 6;
//     // colorPicker.getSvFigCursor().radius += 2;
//     // colorPicker.getWheelCursor().height += 2;

//     // colorPicker.resize(colorPicker.getCanvas().height + 100);
// }
