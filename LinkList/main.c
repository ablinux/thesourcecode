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
    append(30);
    
    /* append */
    append(20);

    /* append */
    append(30);
    
    /* append */
    append(20);

    /* append */
    append(30);
    
    /* append */
    append(20);

    
    display();
    
    return 0;
}