#include "Arduino.h"
#include "DeviceHandle.h"

void setDigitalPin(uint8_t pin, uint8_t value);
uint8_t getDigitalPin(uint8_t pin);
void setPWMValue(uint8_t pin, uint8_t pwmValue);
void sysVariable(uint8_t varIndex, uint8_t *data);
void consolePrint(uint8_t *data);

void sysHandler(NodeFeatureDataBase_t *p_Handle);

/**
 * @brief 
 * 
 * @param p_Handle 
 */
void sysHandler(NodeFeatureDataBase_t *p_Handle)
{
    switch (p_Handle->FunAttr)
    {
    case DIGITAL_PIN_READ:
        getDigitalPin(p_Handle->PinDir);
        break;
    case DIGITAL_PIN_WRITE:
        setDigitalPin(p_Handle->FunAttr,p_Handle->PinDir);

    }
}

/**
 * @brief Set the Digital Pin object
 * 
 * @param pin 
 * @param value 
 */
void setDigitalPin(uint8_t pin, uint8_t value)
{
}
/**
 * @brief Get the Digital Pin object
 * 
 * @param pin 
 * @return uint8_t 
 */
uint8_t getDigitalPin(uint8_t pin)
{
}
/**
 * @brief 
 * 
 * @param pin 
 * @param pwmValue 
 */
void setPWMValue(uint8_t pin, uint8_t pwmValue)
{
}
/**
 * @brief 
 * 
 * @param varIndex 
 * @param data 
 */
void sysVariable(uint8_t varIndex, uint8_t *data)
{
}

/**
 * @brief 
 * 
 * @param data 
 */
void consolePrint(uint8_t *data)
{
}
