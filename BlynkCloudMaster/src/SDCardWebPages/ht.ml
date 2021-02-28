
 <!DOCTYPE html>
 <html>
 <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    
    <style>html
    {
        font-family: Helvetica;
        display: inline-block;
        margin: 0px auto;
        text-align: center;
    }

    .slidecontainer
    {
       width: 100%;
    }
    .slider {
      -webkit-appearance: none;
      width: 100%;
      height: 25px;
      background: #d3d3d3;
      outline: none;
      opacity: 0.7;
      -webkit-transition: .2s;
      transition: opacity .2s;
    }
    .slider:hover {
      opacity: 1;
    }
    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 25px;
      height: 25px;
      background: #4CAF50;
      cursor: pointer;
    }

    .slider::-moz-range-thumb {
      width: 25px;
      height: 25px;
      background: #4CAF50;   
      cursor: pointer;
    }
     .button
    {
        background-color: #195B6A;
        border: none;
        color: white;
        padding: 16px 40px;
        text-decoration: none;
        font-size: 30px;
        margin: 2px;
        cursor: pointer;
    }
    .button2
    {
       background-color: #77878A;
    }
    </style>
</head>
<body>
     <img src="HARLO.jpg" alt="Trulli" width="200" height="80">
     <h1> AbhiNidhi Homes control</h1>
     <div class="slidecontainer">
        <p>Custom range slider:</p>
        <input type="range" min="1" max="100" value="50" class="slider" id="myRange">
        <p>Value: <span id="demo"></span></p>
        </div>
        
        <p>Room Temperature: <span id="temp">updating..</span></p>
        
        <p>TV LED IS: <span id="tv_led_state">updating...</span></p>
        <p><button class="button" onclick="TVLEDfunction()">TV LED</button></p>
        
        <p>TOP Section LED IS: <span id="top_state">updating...</span></p>
        <p><button class="button" onclick="TOPLEDfunction()">TOP LED</button></p>
        
        <p>BED ROOM LED IS: <span id="bed_led_tv">updating...</span></p>
        <p><button class="button" onclick="BEDLEDfunction()">BEDROOM LED</button></p>
</body>
<script>

var range = document.getElementById("myRange");
range.oninput = function() {
    console.log(this.value);
    var xhr = new XMLHttpRequest();
    var url = "/Slider" ;
    xhr.onreadystatechange = function() {
     if (this.readyState == 4 && this.status == 200) {
        document.getElementById("demo").innerHTML = this.value;
      }
    }
    document.getElementById("demo").innerHTML = this.value;
    xhr.open("POST", url, true);
}
function TVLEDfunction()"
{
console.log("button clicked");
var xhr = new XMLHttpRequest();
var url = "/tvled";
xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status==200){
      document.getElementById("tv_led_state").innerHTML = this.responseText;
    }
  }
  xhr.open("GET", url, true);
  xhr.send();
}
// TOP LED Script
function TOPLEDfunction()"
{
console.log("button clicked");
var xhr = new XMLHttpRequest();
var url = "/topled";
xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status==200){
      document.getElementById("top_state").innerHTML = this.responseText;
    }
  }
  xhr.open("GET", url, true);
  xhr.send();
}
// BED ROOM led script
function BEDLEDfunction()"
{
console.log("button clicked");
var xhr = new XMLHttpRequest();
var url = "/bedroomled";
xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status==200){
      document.getElementById("bed_led_tv").innerHTML = this.responseText;
    }
  }
  xhr.open("GET", url, true);
  xhr.send();
}
setInterval(function() {
  // Call a function repetatively with 2 Second interval
  getData();
}, 8000);

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

 document.addEventListener('DOMContentLoaded', TVLEDfunction, false);
 document.addEventListener('DOMContentLoaded', TOPLEDfunction, false);
 document.addEventListener('DOMContentLoaded', BEDLEDfunction, false);
 </script>
 </html>;