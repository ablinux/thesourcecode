#include "RFComms.h"
#include <nRF24L01.h>
#include <RF24.h>

extern RF24 radio;

uint8_t Read(char *deviceID, uint8_t *readBuffer, uint8_t size)
{
    radio.read(readBuffer,size);
}

uint8_t Write(const byte *deviceID, uint8_t *writeBuffer, uint8_t size)
{
    radio.stopListening();
    radio.openWritingPipe(deviceID);
    radio.write(writeBuffer,size);
    radio.startListening();
}