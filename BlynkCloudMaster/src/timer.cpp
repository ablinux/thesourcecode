#include "timer.h"

/* private function to file */
static void push(Node_t **head_ref, callback_t callback);
static void deleteIteam(Node_t **head_ref, callback_t callback_key);
static void RunTask(Node_t *node);

/* Globle static variable */
static volatile uint32_t sysTickValue = 0;
static volatile uint32_t timerEvent = 0;

/* Create a list for 100, 500, 1000, 2000, 5000, event callbacks */
Node_t *p_Listof100MS;
Node_t *p_Listof500MS;
Node_t *p_Listof1000MS;
Node_t *p_Listof2000MS;
Node_t *p_Listof5000MS;

/* initialize the timer for 100ms interrupt 
Source code taken from http://www.8bit-era.cz/arduino-timer-interrupts-calculator.html */
void initTimer100MS()
{
    // // TIMER 1 for interrupt frequency 10 Hz:
    // cli();      // stop interrupts
    // TCCR1A = 0; // set entire TCCR1A register to 0
    // TCCR1B = 0; // same for TCCR1B
    // TCNT1 = 0;  // initialize counter value to 0
    // // set compare match register for 10 Hz increments
    // OCR1A = 24999; // = 16000000 / (64 * 10) - 1 (must be <65536)
    // // turn on CTC mode
    // TCCR1B |= (1 << WGM12);
    // // Set CS12, CS11 and CS10 bits for 64 prescaler
    // TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10);
    // // enable timer compare interrupt
    // TIMSK1 |= (1 << OCIE1A);
    // sei(); // allow interrupts
}

void setTimedRoutine(callback_t callback_function, timedEventTrigger_t timeEvent)
{
    switch (timeEvent)
    {
    case TIMER_TRIGGER_100MS:
        push(&p_Listof100MS, callback_function);
        break;
    case TIMER_TRIGGER_500MS:
        push(&p_Listof500MS, callback_function);
        break;
    case TIMER_TRIGGER_1000MS:
        push(&p_Listof1000MS, callback_function);
        break;
    case TIMER_TRIGGER_2000MS:
        push(&p_Listof2000MS, callback_function);
        break;
    case TIMER_TRIGGER_5000MS:
        push(&p_Listof5000MS, callback_function);
        break;
    default:
        break;
    }
}

void rmTimedRoutineFunction(callback_t callback, timedEventTrigger_t timeEvent)
{
    switch (timeEvent)
    {
    case TIMER_TRIGGER_100MS:
        deleteIteam(&p_Listof100MS, callback);
        break;
    case TIMER_TRIGGER_500MS:
        deleteIteam(&p_Listof500MS, callback);
        break;
    case TIMER_TRIGGER_1000MS:
        deleteIteam(&p_Listof1000MS, callback);
        break;
    case TIMER_TRIGGER_2000MS:
        deleteIteam(&p_Listof2000MS, callback);
        break;
    case TIMER_TRIGGER_5000MS:
        deleteIteam(&p_Listof5000MS, callback);
        break;
    default:
        break;
    }
}

/* Update the 100ms timer tick */
void runTimedEvent()
{
    cli();
    if(sysTickValue % TIMER_TRIGGER_100MS == 0)
        RunTask(p_Listof100MS);
    if(sysTickValue % TIMER_TRIGGER_500MS == 0)
        RunTask(p_Listof500MS);
    if(sysTickValue % TIMER_TRIGGER_1000MS == 0)
        RunTask(p_Listof1000MS);
    if(sysTickValue % TIMER_TRIGGER_2000MS == 0)
        RunTask(p_Listof2000MS);
    if(sysTickValue % TIMER_TRIGGER_5000MS == 0)
        RunTask(p_Listof5000MS);    
    // Serial.print("Systick : ");Serial.println(sysTickValue);
    sei();
    delay(100); /* Need more optimized way of doing this */
}
/* Given a reference (pointer to pointer) to the head of a list 
   and a key, deletes the first occurrence of key in linked list */
static void deleteIteam(Node_t **head_ref, callback_t callback_key)
{
    // Store head node
    Node_t *temp = *head_ref, *prev;

    // If head node itself holds the key to be deleted
    if (temp != NULL && temp->callback == callback_key)
    {
        *head_ref = temp->next; // Changed head
        free(temp);             // free old head
        return;
    }

    // Search for the key to be deleted, keep track of the
    // previous node as we need to change 'prev->next'
    while (temp != NULL && temp->callback != callback_key)
    {
        prev = temp;
        temp = temp->next;
    }

    // If key was not present in linked list
    if (temp == NULL)
        return;

    // Unlink the node from linked list
    prev->next = temp->next;

    free(temp); // Free memory
}
/* Given a reference (pointer to pointer) to the head of a list 
   and an int, inserts a new node on the front of the list. */
static void push(Node_t **head_ref, callback_t callback)
{
    Node_t *new_node = (Node_t *)malloc(sizeof(Node_t));
    if (new_node == NULL)
    {
        Serial.println("Malloc Failed");
    }
    new_node->callback = callback;
    new_node->next = (*head_ref);
    (*head_ref) = new_node;
}

/* run the callbacks from the list */
static void RunTask(Node_t *node)
{
    while (node != NULL)
    {
        // Serial.println("Running the Task...");
        node->callback();
        node = node->next;
    }
}

// Timer 0 is not usable for requested frequency.

// ISR(TIMER1_COMPA_vect)
// {
//     //interrupt commands for TIMER 1 here
//     sysTickValue += 100;
// }

// Timer 2 is not usable for requested frequency.