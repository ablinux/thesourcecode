#ifndef DH_H_
#define DH_H_

#include <Arduino.h>

#define TV       2
#define FISHTANK 3

typedef uint8_t deviceID_t;
typedef uint8_t dataByte_t;
typedef uint8_t action_t;

typedef struct NodeOneInterface
{
   deviceID_t deviceID;
   dataByte_t dataByte;
   action_t action;
}DeviceOne_t;

void handleAck          (uint8_t* data);
void handleAcuatorDevice(uint8_t* data);
void sendAck            (uint8_t* data);
void handleUpdate       (uint8_t* data);
void handleSensorDevice (uint8_t* data);

    
#endif 