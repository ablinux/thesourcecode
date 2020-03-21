#include "airCommandHandler.h"
#include "DevReg.h"
#include "RFComms.h"

static bool notified = false;
static bool getNotification ();
extern void BlinkLed();

/* Notification handler */
void airNotificationHandler(airMail_t *p_mail)
{
    /* only consume the CPU if you have received the message else just fuck off */
    if(!getNotification ()) {  return;}
    else{   readNotify(false);}
    
    switch(p_mail->mailHeader.pktType)
    {
        case PKT_ACK:
        Serial.println("ACK Packet");
        handleACK();
        break;
        case PKT_DEVICE_ACTUATOR:
        Serial.println("Device Actuator");
        break;
        case PKT_DEVICE_SENSOR:
        Serial.println("Device Sensor");
        handleSensorDevice();
        break;
        case PKT_REGISTRATION:
        Serial.println("Registration Packet");
        // if(registerDevice(p_mail,&NodeDescripter[NodeID]))
        // {
        //     // Write(NodeDescripter[NodeID-1].NodeAddress,)
        // }
        break;
        case PKT_UPDATE:
        Serial.println("Bootloader Packet");
        handleUpdate();
        break;
        default:
        Serial.println("Un-Identified packet");
        break;
    }
}

void handleACK()
{
    /* */

}
void handleAcuatorDevice()
{

}
void handleDeviceReg()
{

}
void handleUpdate()
{

}
void handleSensorDevice()
{

}

void readNotify(bool isNotify)
{
    notified = isNotify;
}

static bool getNotification ()
{
    return notified;
}