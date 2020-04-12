#include "airCommandHandler.h"
#include "DevReg.h"
#include "RFComms.h"

static bool notified = false;
static bool getNotification ();
extern void BlinkLed();
extern char TempInC[];
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
        handleACK(p_mail);
        break;
        case PKT_DEVICE_ACTUATOR:
        Serial.println("Device Actuator");
        break;
        case PKT_DEVICE_SENSOR:
        Serial.println("Device Sensor");
        handleSensorDevice(p_mail);
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

void handleACK(airMail_t *p_mail)
{
    /* */
    static int i=0;
    display.setCursor(0,23);display.setTextColor(WHITE,BLACK);display.printf("Node addr: %s \nCount [%d]",p_mail->data,i);display.display();
    i++;
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
void handleSensorDevice(airMail_t *p_mail)
{
    display.setCursor(0,49);display.setTextColor(WHITE,BLACK);display.printf("Room Temp:%s",p_mail->data);display.display();
    memcpy(TempInC,p_mail->data,5);
}

void readNotify(bool isNotify)
{
    notified = isNotify;
}

static bool getNotification ()
{
    return notified;
}