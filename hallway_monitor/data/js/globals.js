/**
 * simple wrapper to avoid having to use (typeof(value) === 'undefined') everywhere
 * 
 * @param {*} value
 * @returns {boolean}
 */
function isUndefined(value) {
    // Obtain `undefined` value that's
    // guaranteed to not have been re-assigned
    var undefined = void(0);
    return value === undefined;
}



/**
 * Generic method for ajax posts
 *
 * success call backs only fire if both the AJAX and Server Side operations were successful.
 *    onSuccess = function({object} data) {};
 *
 * error call backs fire if either the AJAX or Server Side operations failed
 *    onError = function({string} error, {object} data) {};
 *
 * complete call backs fire always
 *    onComplete = function() {};
 *
 * @param {string} route
 * @param {object} [data={}]
 * @param {string} [method="POST"]
 * @param {function|undefined} [baseDataCallback=undefined] pass an object function(){}; which returns a base data struct common to all ajax calls to remove duplication of base data
 * @param {function|undefined} [lockInterfaceCallback=undefined] pass a void function(boolean){}; which performs any interface locking requirements at the appropriate times
 * @param {function|undefined} [successCallback=undefined]
 * @param {function|undefined} [errorCallback=undefined]
 * @param {function|undefined} [completeCallback=undefined]
 * @param {boolean} [expectSuccessResponse=true]
 */
ajaxPost = function(route, data, method, baseDataCallback, lockInterfaceCallback, successCallback, errorCallback, completeCallback, expectSuccessResponse) {
    try {
        var jsonData = {};
        if (!isUndefined(baseDataCallback)) {
            jsonData = baseDataCallback();
        }
        if (!isUndefined(data)) {
            $.extend(jsonData, data);
        }

        $.ajax({
            type: !isUndefined(method) ? method : 'POST',
            url: route,
            data: jsonData,
            dataType: 'json',

            beforeSend: function() {
                if (!isUndefined(lockInterfaceCallback)) {
                    lockInterfaceCallback(true);
                }
            },

            //Ajax Call succeeded but doesn't necessarily mean the server side operation completed
            success: function(data, textStatus, jqXHR) {
                //Evaluate the data and call the error callback if it didn't work
                if ((!isUndefined(expectSuccessResponse) && (expectSuccessResponse!==true)) || data['success']) {
                    if (!isUndefined(successCallback)){
                        successCallback(data);
                    }
                } else {
                    if (!isUndefined(errorCallback)){
                        errorCallback(data['error'], data);
                    } else {
                        console.error('ajax call failed: "' + route + '" ' + JSON.stringify(data), 2);
                        console.error(' - error:' + data['error']);
                    }
                }

            },

            error: function(jqXHR, textStatus, errorThrown) {
                //On Error Callbacks
                if (!isUndefined(errorCallback)){
                    if (!isUndefined(jqXHR['responseJSON']) && !isUndefined(jqXHR['responseJSON']['error'])) {
                        errorCallback(jqXHR['responseJSON']['error']);
                    } else {
                        errorCallback(textStatus);
                    }
                } else {
                    console.error('ajax call failed: "' + route + '" ' + JSON.stringify(data), 2);
                    console.error(' - textStatus:' + textStatus);
                    console.error(' - errorThrown:' + errorThrown, 2);
                }
            },

            complete: function(jqXHR, textStatus) {
                if (!isUndefined(completeCallback)){
                    completeCallback();
                }

                if (!isUndefined(lockInterfaceCallback)) {
                    lockInterfaceCallback(false);
                }
            }
        });
    } catch (ex) {
        console.error(ex, 2);

        if (!isUndefined(errorCallback)){
            errorCallback();
        }

        if (!isUndefined(completeCallback)){
            completeCallback();
        }
    }
};