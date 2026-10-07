// bdc 0x08a317d8 NetErrorListRewind
#include "bdc.h"

/* Resets the walk cursor of a net-error list to the node after the `active` chain head. Called
   by `NetErrorListMerge`. */
void NetErrorListRewind(CorePrioList *list)
{
    list->cursor = NetErrorNodeGetNext(list->active);
}
