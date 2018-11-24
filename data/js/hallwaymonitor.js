var colorPicker = new iro.ColorPicker('#colorPicker', {
    width: 360,
    height: 360,
    markerRadius: 8,
    color: '#fff',
    borderWidth: 2,
    padding: 4,
    anticlockwise: true
});

function getAjax(url, success) {
    var xhr = window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject('Microsoft.XMLHTTP');
    xhr.open('GET', url);
    xhr.onreadystatechange = function() {
        if (xhr.readyState>3 && xhr.status==200) success(xhr.responseText);
    };
    xhr.setRequestHeader('X-Requested-With', 'XMLHttpRequest');
    xhr.send();
    return xhr;
}

function colorChangeHandler(color) {
    console.log(color.rgb)
    let queryString = `r=${color.rgb.r}&g=${color.rgb.g}&b=${color.rgb.b}`;
    getAjax('rgb?' + queryString, function(data){
        console.log(data); 
    });
}

colorPicker.on("input:end", colorChangeHandler)