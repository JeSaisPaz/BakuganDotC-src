// bdc 0x089d8e24 CoreNodeChainCount
#include "bdc.h"

/* Counts the nodes of a sibling chain by following `next` from `first`; returns 0 for a NULL chain.
    */
s32 CoreNodeChainCount(CoreNode *first)
{
    s32 count = 0;

    for (; first != NULL; first = first->next) {
        count++;
    }
    return count;
}
