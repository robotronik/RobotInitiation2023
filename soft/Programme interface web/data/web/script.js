var joy = new JoyStick('joy1Div',{

    // The ID of canvas element
    title: 'joystick',

    // width/height
    width: undefined,
    height: undefined,

    // Internal color of Stick
    internalFillColor: '#AA0000',

    // Border width of Stick
    internalLineWidth: 2,

    // Border color of Stick
    internalStrokeColor: '#000000',

    // External reference circonference width
    externalLineWidth: 10,

    //External reference circonference color
    externalStrokeColor: '#4b4b4',

    // Sets the behavior of the stick
    autoReturnToCenter: true,
    
});

setInterval(function(){ 
    sendValueX(joy.GetX()); 
    sendValueY(joy.GetY()); 
}, 100);

function sendValueX(value) {
    console.log(value)
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "positionX?value=" + value, true);
    xhttp.send();
}

function sendValueY(value) {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "positionY?value=" + value, true);
    xhttp.send();
}

function RevButton() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "REV", true);
    xhttp.send();
}

function FwdButton() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "FWD", true);
    xhttp.send();
}


function offButton() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "off", true);
    xhttp.send();
}