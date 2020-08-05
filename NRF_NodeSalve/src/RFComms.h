#ifndef RFCOMMS_H_
#define RFCOMMS_H_

uint8_t Read (char *deviceID, uint8_t *readBuffer, uint8_t size);
uint8_t Write(const byte *deviceID, uint8_t *writeBuffer, uint8_t size);

#endif