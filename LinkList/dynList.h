#ifndef DY_LIST_H_
#define DY_LIST_H_

#include <stdint.h>
#include <stddef.h>
#include<stdio.h>
typedef struct LList 
{
    struct LList *next;
    uint32_t data;
}LList_t;

/* Init the list*/
void initLList(LList_t *start);

/* Append the item at the end of the list */
uint8_t append(uint32_t data);

/* Remove the time from the end of the list*/
uint8_t removeFromEnd(void);

/* Remove List from user provided index*/
uint8_t removeItem(uint8_t index);

/* Add the item at user provided index*/
uint8_t add(uint32_t *data, uint8_t index);

/* Add at the begining of the list*/
uint8_t addAtTop(uint32_t *data);

/* Get the number of items*/
uint8_t count();

/* iter the item*/
void display();

uint8_t pop(uint8_t index);

uint8_t push(LList_t *node);

#endif