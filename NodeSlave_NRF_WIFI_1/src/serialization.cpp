#include <Arduino.h>
#include "serialization.h"

uint8_t * serialize_32byte(uint8_t *buffer, uint32_t value)
{
  /* Write big-endian int value into buffer; assumes 32-bit int and 8-bit char. */
  buffer[0] = value >> 24;
  buffer[1] = value >> 16;
  buffer[2] = value >> 8;
  buffer[3] = value;
  return buffer + 4;
}

uint8_t * serialize_8byte(uint8_t *buffer, uint8_t value)
{
  buffer[0] = value;
  return buffer + 1;
}

uint8_t * serialize_airmailHeader(uint8_t *buffer,airMailHeader_t *p_mail_header)
{
  buffer = serialize_8byte(buffer,p_mail_header->pktType);
  buffer = serialize_8byte(buffer,p_mail_header->dataLength);
  return buffer;
}

uint8_t * serialize_airmail(uint8_t *buffer, airMail_t *p_mail, uint8_t dataLength)
{
  buffer = serialize_8byte(buffer, p_mail->mailHeader.pktType);
  buffer = serialize_8byte(buffer, p_mail->mailHeader.dataLength);
  buffer = serialize_8byte(buffer, p_mail->airCommand);
  uint8_t i = 0;
  while(i<dataLength)
  {
    buffer = serialize_8byte(buffer, p_mail->data[i]);
    i++;
  }
  return buffer;
}

void de_serialize_header(airMailHeader_t *mailHeader,uint8_t *srcByteStream)
{
    mailHeader->pktType = srcByteStream[0];
    mailHeader->dataLength = srcByteStream[1];
}

void de_serialize_airmail(airMail_t *airmail,uint8_t *srcByteStream)
{
    uint8_t i = 0,j=0;
    airmail->mailHeader.pktType = srcByteStream[i++];
    airmail->mailHeader.dataLength = srcByteStream[i++];
    airmail->airCommand = srcByteStream[i++];
    /* Get the length of databuffer */
    while (j < airmail->mailHeader.dataLength)
    {
        airmail->data[j++] = srcByteStream[i++];
    }
}