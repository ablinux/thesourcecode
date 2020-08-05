#ifndef WEB_HANDLER_
#define WEB_HANDLER_
#include "common.h"
#include "sysCfg.h"

void TVLEDState();
void TOPLEDState();
void BEDLEDState();
void handleTemp();
void handleSlider();
void handleTime();
void sendNodeStatus();

#endif