var i = 1;

function OnOff_Handler(Device) {
    var checkbox = document.getElementById(Device);
    switch (Device) {
        case 'TV':
            if (checkbox.checked == true) {
                console.log("Device " + Device + " is Off");
            } else {
                console.log("Device " + Device + " is ON");
            }
            break;
        case 'Lamp':
            if (checkbox.checked == true) {
                console.log("Device " + Device + " is Off");
            } else {
                console.log("Device " + Device + " is ON");
            }
    }
}

function addNodeDevice() {
    //Create an input type dynamically.
    var label = document.createElement("label");
    var input = document.createElement("input");
    var dev = document.createElement("dev");
    var body = document.createElement("body")
        //Assign different attributes to the element.
    label.setAttribute("class", "switch-wrap");

    input.setAttribute("type", "checkbox");
    input.setAttribute("id", "Lamp");

    dev.setAttribute("class", "switch");
    dev.setAttribute("onclick", "OnOff_Handler('Lamp')");

    var foo = document.getElementById("ButtonLists");

    //Append the element in page (in span).
    foo.appendChild(body);
    body.appendChild(label);
    label.appendChild(input);
    label.appendChild(dev);
}

function addNodeDeviceInTable() {
    var label = document.createElement("label");
    var input = document.createElement("input");
    var dev = document.createElement("dev");
    var dev = document.createElement("tr");
    var foo = document.getElementById("TableList");
}