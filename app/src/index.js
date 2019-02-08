// @TODO: sockets. Ajax Sux
const ControlPanel = require("./pages/controlpanel");
var pageControl;

/**
 * Fired when the document is loaded and ready
 */
$( document ).ready(function() {
    pageControl = new ControlPanel();
});