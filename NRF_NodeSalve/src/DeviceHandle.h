#ifndef DH_H_
#define DH_H_

#include <Arduino.h>

#define TV       2
#define FISHTANK 3
typedef uint8_t deviceID_t;
typedef uint8_t dataByte_t;
typedef uint8_t action_t;

typedef uint8_t nodeAddress_t;            /* Node with featues */
typedef uint8_t nodeStatus_t;             /* Node Online=1 or Offline=0 */
typedef uint8_t functionName_t;           /* What is it about TV? FAN? Sensor?*/
typedef uint8_t functionAttribute_t;      /* in case of Actuator device it will be a pin number ex Pin connected to PIN7 etc */
typedef uint8_t funAttrDirection_t;       /* with pin do i have to read to write */

typedef struct NodeOneInterface
{
   deviceID_t deviceID;
   dataByte_t dataByte;
   action_t action;
}DeviceOne_t;

typedef struct NodeFeatueDataBase
{
   nodeAddress_t           NodeAddress[5];  /* Address of Device Node Limit=5bytes*/ 
   nodeStatus_t            NodeStatus;      /* Online=1 or Offline */
   functionName_t          Function[16];    /* Example "TV" or "FAN", Limit=16Bytes */
   functionAttribute_t     FunAttr;         /* Example Pin number of function */
   funAttrDirection_t      FunDir;          /* Direction of Pin Read? or Write */
}NodeFeatureDataBase_t;

#define JSON_STR_NODE_FEATURE_TO_WEB "{\"DVAD\":\"%s\"}"

extern NodeFeatureDataBase_t NodeFeatureDataBaseLog;

void handleAck          (uint8_t* data);
void OnOffDevice(uint8_t* data);
void sendAck            (uint8_t* data);
void handleUpdate       (uint8_t* data);
void handleSensorDevice (uint8_t* data);

    
#endif 