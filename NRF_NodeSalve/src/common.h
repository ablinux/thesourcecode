#ifndef _COMMON_H_
#define _COMMON_H_

#include "Arduino.h"

extern void     setDigitalPin   (uint8_t pin, uint8_t value);
extern uint8_t  getDigitalPin   (uint8_t pin);
extern void     setPWMValue     (uint8_t pin, uint8_t pwmValue);
extern void     sysVariable     (uint8_t varIndex, uint8_t* data);

#endif