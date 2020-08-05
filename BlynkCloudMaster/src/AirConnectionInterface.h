#ifndef AIR_INTERFACE_H_
#define AIR_INTERFACE_H_

#include <Arduino.h>

/* Device registration */
typedef struct deviceReg
{
    uint8_t NodeAddress[5];
}NodeDescripter_t;

/* AirPayload structure */
#define MAX_PAYLOAD_SIZE 16
#define MAX_NODE_SUPPORTED 20
typedef uint8_t pktType_t; 
typedef uint8_t airCommand_t;

enum packetType
{
    /* Commands from Master to node */
    PKT_DEVICE_SENSOR = 0x01,
    PKT_DEVICE_ACTUATOR,
    PKT_UPDATE,
    PKT_REGISTRATION,
    PKT_ACK
    /* Commands from NODE to master */
    
    /* Custom command */

};

/* packet type */
enum airCommand 
{

    COMMAND_SET = 0x1,
    COMMAND_GET,
    COMMAND_NOTIFY,
    COMMAND_ERROR,
    COMMAND_REG,
    COMMAND_DREG

};


/* PayLoad Header
 Description : Packet header contains the information about the payload */
typedef struct Header
{
    pktType_t pktType; 
    uint8_t dataLength;

}airMailHeader_t;
#define AIR_MAIL_HEADER_SIZE (sizeof(airMailHeader_t))

/* Payload */
typedef struct Payload
{
    airMailHeader_t mailHeader;
    airCommand_t    airCommand;
    uint8_t data[MAX_PAYLOAD_SIZE]; /* holds the data */

}airMail_t;


extern NodeDescripter_t NodeDescripter[MAX_NODE_SUPPORTED];

#define HEADER_SIZE sizeof(airMailHeader_t)

#endif