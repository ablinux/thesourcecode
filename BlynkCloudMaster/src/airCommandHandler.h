#ifndef AIR_HANDLE_
#define AIR_HANDLE_
#include "AirConnectionInterface.h"
#include "command.h"

void airNotificationHandler(airMail_t *p_mail);
void readNotify(bool );

void handleACK(airMail_t *p_mail);
void handleAcuatorDevice();
void handleDeviceReg();
void handleUpdate();
void handleSensorDevice(airMail_t *p_mail);

#endif