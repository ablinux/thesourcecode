#ifndef RMTN_H
#define RMTN_H

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


#endif