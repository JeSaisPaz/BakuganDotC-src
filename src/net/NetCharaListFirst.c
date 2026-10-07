// bdc 0x08a31540 NetCharaListFirst
#include "bdc.h"

/* Returns the first node of the net-character list `list` (the node after its sentinel), or
   NULL when the list has no sentinel yet. Used by the net character manager to
   start its walks over the characters. */
void *NetCharaListFirst(void *list)
{
    const CoreList *cbList = list;

    if (cbList->sentinel == NULL) {
        return NULL;
    }
    return cbList->sentinel->next;
}
