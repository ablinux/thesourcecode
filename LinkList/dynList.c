#include "dynList.h"
#include "stdlib.h"
/* Local vars */
static LList_t *Start =  NULL;
static LList_t *End   =  NULL;

/* Init the list*/
void initLList(LList_t *start)
{
    start->next = NULL;
    Start = start;
    End->next = NULL;
    Start = End;
}

/* Append the item at the end of the list */
uint8_t append(uint32_t data)
{
    LList_t *curr=NULL,*prv=NULL;
    
    LList_t *NewNode = (LList_t*)malloc(sizeof(LList_t));
    NewNode->data = data;

    /* If its a 1st node */
    if(Start->next == NULL)
    {
        Start = NewNode;
        End = NewNode;
        printf("1st item\n");
    }
    else
    {
        /* Iter to the end of the list */
        curr = Start;
        while (curr->next != NULL)
        {
            prv = curr;
            curr = curr->next;
        }
        /* at this point i have end item and prv item */
        prv->next = NewNode;
        End = NewNode;
        printf("Other Items\n");
        
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
    LList_t *ItrNode = Start;
    while (ItrNode->next != NULL)
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
