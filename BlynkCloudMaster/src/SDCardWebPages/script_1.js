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