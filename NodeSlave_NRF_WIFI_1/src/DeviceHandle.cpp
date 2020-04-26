#include "DeviceHandle.h"
#include "Adafruit_SSD1306.h"
#include "airCommandHandler.h"

extern Adafruit_SSD1306 display;
extern uint8_t isDispOn;
airCommandHandler_t handlers ={
    p_call_handleAck : handleAck,
    p_call_OnOffDevice : OnOffDevice,
    p_call_sendAck : NULL,
    p_call_handleUpdate : NULL,
    p_call_handleSensorDevice : handleSensorDevice
};

void handleAck(uint8_t *data)
{
}

void OnOffDevice(uint8_t *data)
{
    
}

void sendAck(uint8_t *data)
{
}

void handleUpdate(uint8_t *data)
{
}

void handleSensorDevice(uint8_t *data)
{
   
}