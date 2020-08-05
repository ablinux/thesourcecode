// Send Temp Value
setInterval(function() {
    // myFunction('Intervel', 'Online')
    getDevice()

}, 2000);

function getDevice() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            console.log(this.responseText);
            var obj = JSON.parse(this.responseText);
            myFunction(obj.DVAD, obj.DVST);
        }
    }
    xhttp.open("GET", "getDeviceList", true);
    xhttp.send();
}
var serialNumber = 0;
var list = [];

function myFunction(deviceAddress, status) {

    var isAddressFound = list.indexOf(deviceAddress, 0);
    if (isAddressFound == -1) {
        list.push(deviceAddress);
        console.log(list);
        serialNumber++;
        var table = document.getElementById("DeviceList");
        var row = table.insertRow();
        var SN = row.insertCell();
        var DAdd = row.insertCell();
        var Status = row.insertCell();
        SN.innerHTML = serialNumber;
        DAdd.innerHTML = deviceAddress;
        Status.innerHTML = "Online";
    }
}