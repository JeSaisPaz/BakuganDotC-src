// bdc 0x08a299b8 BtlStatsListFirst
#include "bdc.h"

/* Returns the first node of a `CoreCallbackListInit` list (the `CoreList` sentinel's `next`)
   or NULL when the list has no sentinel. Second compiled instance of the engine list class next to
   `CoreListFirst`; the nodes of this instance carry callback function pointers in their payload
   word. */
void *BtlStatsListFirst(void *list)
{
    const CoreList *cbList = list;

    if (cbList->sentinel == NULL) {
        return NULL;
    }
    return cbList->sentinel->next;
}
