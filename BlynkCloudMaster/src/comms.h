#ifndef _COMMS_H_
#define _COMMS_H_

#include "common.h"
#include "AirConnectionInterface.h"

bool sendData(uint8_t *nodeAddress, uint8_t *data, uint8_t dataLen);
bool recvData(uint8_t *nodeAddress, uint8_t* data, uint8_t *dataLen);


#endif