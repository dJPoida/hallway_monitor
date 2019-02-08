const ConfigureAP = require("./pages/configureap");
var pageControl;

/**
 * Fired when the document is loaded and ready
 */
$( document ).ready(function() {
    pageControl = new ConfigureAP();
});