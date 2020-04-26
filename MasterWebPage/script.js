var range = document.getElementById("myRange_1");
range.onchange = function() {
    console.log(this.value);
    var xhr = new XMLHttpRequest();
    var sliderValue = this.value;
    var url = "/Slider";
    xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("demo_1").innerHTML = sliderValue;
        }
        //console.log(sliderValue);
    }
    document.getElementById("demo_1").innerHTML = sliderValue;
    xhr.open("GET", "/Slider?SliderVal=" + sliderValue, true);
    xhr.send();
}

var range = document.getElementById("myRange_2");
range.oninput = function() {
    console.log(this.value);
    var xhr = new XMLHttpRequest();
    var sliderValue = this.value;
    var url = "/Slider";
    xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("demo_2").innerHTML = sliderValue;
        }
        //console.log(sliderValue);
    }
    document.getElementById("demo_2").innerHTML = sliderValue;
    xhr.open("GET", "/Slider_1?SliderVal=" + sliderValue, true);
    xhr.send();
}

var range = document.getElementById("myRange_3");
range.oninput = function() {
    console.log(this.value);
    var xhr = new XMLHttpRequest();
    var sliderValue = this.value;
    var url = "/Slider";
    xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("demo_3").innerHTML = sliderValue;
        }
        //console.log(sliderValue);
    }
    document.getElementById("demo_3").innerHTML = sliderValue;
    xhr.open("GET", "/Slider_2?SliderVal=" + sliderValue, true);
    xhr.send();
}

// Range Slider
function TVLEDfunction() {
    console.log("button clicked");
    var xhr = new XMLHttpRequest();
    var url = "/tvled";
    xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("tv_led_state").innerHTML = this.responseText;
        }
    }
    xhr.open("GET", url, true);
    xhr.send();
}

// TOP LED Script
function TOPLEDfunction() {
    console.log("button clicked");
    var xhr = new XMLHttpRequest();
    var url = "/topled";
    xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("top_state").innerHTML = this.responseText;
        }
    }
    xhr.open("GET", url, true);
    xhr.send();
}

// BED ROOM led script
function BEDLEDfunction() {
    console.log("button clicked");
    var xhr = new XMLHttpRequest();
    var url = "/bedroomled";
    xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("bed_led_tv").innerHTML = this.responseText;
        }
    }
    xhr.open("GET", url, true);
    xhr.send();
}

// Send Temp Value
setInterval(function() {
    // Call a function repetatively with 2 Second interval
    getData();
    getTime();
}, 2000);

function getData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("temp").innerHTML = this.responseText;
        }
    }
    xhttp.open("GET", "readADC", true);
    xhttp.send();
}

function getTime() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("time").innerHTML = this.responseText;
        }
    }
    xhttp.open("GET", "getTime", true);
    xhttp.send();
}

document.addEventListener('DOMContentLoaded', TVLEDfunction, false);
document.addEventListener('DOMContentLoaded', TOPLEDfunction, false);
document.addEventListener('DOMContentLoaded', BEDLEDfunction, false);