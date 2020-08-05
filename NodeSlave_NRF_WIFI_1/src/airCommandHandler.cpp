#include "airCommandHandler.h"
// #include "DevReg.h"
// #include "RFComms.h"

#define ERROR_REGISTRATION 0xE12

static airCommandHandler_t *p_callTo=NULL;
static bool notified = false;
static bool getNotification ();
extern void BlinkLed();

/* Register the calles for handling the functions */
void RegisterHandlers(airCommandHandler_t * p_callHandlers)
{
    p_callTo = p_callHandlers;
}

/* Notification handler */
void airNotificationHandler(airMail_t *p_mail)
{
    /* only consume the CPU if you have received the message else just fuck off */
    if(!getNotification ()) {  return;}
    else{   readNotify(false);}

    if(p_callTo == NULL)
        p_mail->mailHeader.pktType =     PKT_ERROR_CALL;
    
    switch(p_mail->mailHeader.pktType)
    {
        case PKT_ACK:
        Serial.println("ACK Packet");
        if(p_callTo->p_call_handleAck)
            p_callTo->p_call_handleAck(p_mail->data);
        else 
            Serial.println("Ack Call is not registed");    
        break;
        
        
        case PKT_DEVICE_ACTUATOR:
        Serial.println("Device Actuator");
        if(p_callTo->p_call_OnOffDevice)
            p_callTo->p_call_OnOffDevice(p_mail->data);
        else
            Serial.println("OnOff call not registed");
        break;
        
        
        case PKT_DEVICE_SENSOR:
        Serial.println("Device Sensor");
        if(p_callTo->p_call_handleSensorDevice != NULL)
            p_callTo->p_call_handleSensorDevice(p_mail->data);
        else
            Serial.println("Device sensor call not registed");
        break;

        
        case PKT_REGISTRATION:
        Serial.println("Registration Packet");
        break;


        case PKT_UPDATE:
        Serial.println("Bootloader Packet");
        if(p_callTo->p_call_handleUpdate)
            p_callTo->p_call_handleUpdate(p_mail->data);
        else
            Serial.println("handleUpdate call not registed");
        break;


        case PKT_ERROR_CALL:
        Serial.println("Error in device registration calls");
        break;


        default:
        Serial.println("Un-Identified packet");
        break;
    }
}

void readNotify(bool isNotify)
{
    notified = isNotify;
}

static bool getNotification ()
{
    return notified;
}