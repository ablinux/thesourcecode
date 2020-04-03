#include "DeviceHandle.h"
#include "Adafruit_SSD1306.h"
#include "airCommandHandler.h"

extern Adafruit_SSD1306 display;

airCommandHandler_t handlers ={
    p_call_handleAck : handleAck,
    p_call_handleAcuatorDevice : NULL,
    p_call_sendAck : NULL,
    p_call_handleUpdate : NULL,
    p_call_handleSensorDevice : handleSensorDevice
};

void handleAck(uint8_t *data)
{
}

void handleAcuatorDevice(uint8_t *data)
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
    uint8_t FishTank_X,TV_x;
    DeviceOne_t *p_device = (DeviceOne_t *)data;
    display.setCursor(0,14);display.print("FISH TANK :");FishTank_X = display.getCursorX();
    display.setCursor(0,21);display.print("TV :");TV_x = display.getCursorX();
    switch (p_device->deviceID)
    {
    case FISHTANK:
        display.setCursor(FishTank_X,14);
        // display.print("FISH TANK :"); 
        display.setTextColor(WHITE,BLACK);
        display.print(p_device->dataByte);  
        display.display();
        Serial.println("FISH TANK :");
        digitalWrite(8,p_device->dataByte);
        break;

    case TV:
        display.setCursor(TV_x,22);
        // display.print("TV :"); 
        display.setTextColor(WHITE,BLACK);
        display.print(p_device->dataByte);  
        display.display();
        Serial.println("TV :");
        digitalWrite(8,p_device->dataByte);
        break;

    default:
        break;
    }
}