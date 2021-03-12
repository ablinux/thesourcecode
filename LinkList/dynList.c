#include "dynList.h"
#include "stdlib.h"
/* Local vars */
static LList_t *Start =  NULL;
static LList_t *End   =  NULL;

/* Init the list*/
void initLList(LList_t *List)
{
    /* Make next address as null */
    List->next = NULL;
    /* Consider the user provided list pointer is the 1st node */
    Start = List;
    /* This is also a End of the list so Start = End */
    End = Start;
}

/* Append the item at the end of the list */
uint8_t append(uint32_t data)
{
    LList_t *NewNode = (LList_t*)malloc(sizeof(LList_t));
    NewNode->data = data;
    NewNode->next = NULL;

    /* If its a 1st node */
    if(Start->next == NULL)
    {
        Start->next = NewNode;
        End = NewNode;
    }
    else
    {
       End->next = NewNode;
       NewNode->next = NULL;
       End = NewNode; 
    }
}

/* Remove the time from the end of the list*/
uint8_t removeFromEnd(void)
{
    

}

/* Remove List from user provided index*/
uint8_t removeItem(uint8_t index)
{

}

/* Add the item at user provided index*/
uint8_t add(uint32_t *data, uint8_t index)
{

}

/* Add at the begining of the list*/
uint8_t addAtTop(uint32_t *data)
{

}

/* Get the number of items*/
uint8_t count()
{

}

/* iter the item*/
void display()
{
    LList_t *ItrNode = Start->next;
    while (ItrNode != NULL)
    {
        printf("|%d|",ItrNode->data);
        ItrNode = ItrNode->next;
    }
}

uint8_t pop(uint8_t index)
{

}

uint8_t push(LList_t *node)
{

}
