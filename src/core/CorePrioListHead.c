// bdc 0x08a30730 CorePrioListHead
#include "bdc.h"

/* Returns the `active` sentinel node (`+0x0`); walkers iterate from its `next`. */
CorePrioNode *CorePrioListHead(CorePrioList *list)
{
    return list->active;
}
