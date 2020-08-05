#ifndef SERIAL_H_
#define SERIAL_H_

#include <Arduino.h>
#include "AirConnectionInterface.h"

uint8_t * serialize_32byte(uint8_t *buffer, uint32_t value);
uint8_t * serialize_8byte(uint8_t *buffer, uint8_t value);
uint8_t * serialize_airmail(uint8_t *buffer, airMail_t *p_mail, uint8_t dataLength);
uint8_t * serialize_airmailHeader(uint8_t *buffer,airMailHeader_t *p_mail_header);

void de_serialize_header(airMailHeader_t *mailHeader,uint8_t *srcByteStream);
void de_serialize_airmail(airMail_t *airmail,uint8_t *srcByteStream);

#endif