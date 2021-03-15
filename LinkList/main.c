#include<stdint.h>
#include<stdio.h>
#include "dynList.h"
int main()
{
    LList_t List;
    printf("LinkList Project\n\r");
    
    /* init the list */
    initLList(&List);

    /* append */
    // append(10);

    removeFromEnd();
    
    return 0;
}