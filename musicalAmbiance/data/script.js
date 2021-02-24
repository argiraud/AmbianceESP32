function onButton() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "on", true);
    xhttp.send();
}

function offButton() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "off", true);
    xhttp.send();
}

function chillButton() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "chillMode", true);
    xhttp.send();
}

function ambianceButton() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "test", true);
    xhttp.send();
}

function colorButton() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "colorMode:", true);
    xhttp.send();
}

function colorBlueButton() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "colorMode/blue", true);
    xhttp.send();
}

function colorGreenButton() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "colorMode/green", true);
    xhttp.send();
}

function colorRedButton() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "colorMode/red", true);
    xhttp.send();
}

function colorYellowButton() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "colorMode/yellow", true);
    xhttp.send();
}


setInterval(function getData()
{
    var xhttp = new XMLHttpRequest();

    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
            document.getElementById("valeurLuminosite").innerHTML = this.responseText;
        }
    };

    xhttp.open("GET", "LireLuminosite", true);
    xhttp.send();
}, 2000);