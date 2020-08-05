#ifndef AIR_HANDLE_
#define AIR_HANDLE_
#include "AirConnectionInterface.h"

typedef struct airCommandHandler
{
    void (*p_call_handleAck)(uint8_t* data);
    void (*p_call_OnOffDevice)(uint8_t* data);
    void (*p_call_sendAck)(uint8_t* data);
    void (*p_call_handleUpdate)(uint8_t* data);
    void (*p_call_handleSensorDevice)(uint8_t* data);
}airCommandHandler_t;

void RegisterHandlers(airCommandHandler_t * p_callHandlers);
void airNotificationHandler(airMail_t *p_mail);
void readNotify(bool );



#endif