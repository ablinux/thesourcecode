#ifndef TIMER_H
#define TIMER_H
#include <Arduino.h>

typedef void (*callback_t)();

/* link list of callbacks */
typedef struct Node
{
    callback_t callback;
    struct Node *next;
} Node_t;

typedef enum
{
    TIMER_TRIGGER_100MS =   100,
    TIMER_TRIGGER_500MS =   500,
    TIMER_TRIGGER_1000MS =  1000,
    TIMER_TRIGGER_2000MS =  2000,
    TIMER_TRIGGER_5000MS =  5000,

} timedEventTrigger_t;

/*this function accepts the pointer to the function that requires to execute at required intervel
and the 2nd paramerter which defines the frequency of occurance */
void setTimedRoutine(callback_t callback, timedEventTrigger_t timeEvent);

/* This function removes the callback function from the list */
void rmTimedRoutineFunction(callback_t callback, timedEventTrigger_t timeEvent);

/* Init the timer for 100MS */
void initTimer100MS();

/* Update the timer variable */
void runTimedEvent();
#endif