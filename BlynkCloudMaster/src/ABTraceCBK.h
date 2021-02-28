#ifndef ABTRACECBK_H_
#define ABTRACECBK_H_

#include <stdint.h>

/* TraceCallbackStruct */
typedef struct ABTraceCallback
{
    void (*callback)(uint8_t len,uint8_t *data);
}ABTraceCallback_t;

#endif