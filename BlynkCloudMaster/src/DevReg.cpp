#include "DevReg.h"

extern uint8_t NodeID;

uint8_t registerDevice(airMail_t *p_mail,NodeDescripter_t *Node)
{
    if(p_mail->data[0] == 0)
        return 0;
    memcpy(Node->NodeAddress,p_mail->data,5);
    NodeID++;
    return 1;
}