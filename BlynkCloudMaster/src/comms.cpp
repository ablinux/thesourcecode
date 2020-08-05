#include "comms.h"
#include "RF24.h"
#include <nRF24L01.h>

extern RF24 radio;

/**
 * @brief 
 * 
 * @param nodeAddress 
 * @param data 
 * @return true 
 * @return false 
 */
bool sendData(uint8_t *nodeAddress, uint8_t *data, uint8_t dataLen)
{
    radio.stopListening();
    radio.openWritingPipe(nodeAddress);
    radio.write(data, dataLen);
    radio.startListening();
    return true;
}

/**
 * @brief 
 * 
 * @param nodeAddress 
 * @param data 
 * @return true 
 * @return false 
 */
bool recvData(uint8_t *nodeAddress, uint8_t* data, uint8_t *dataLen)
{
    return true;
}