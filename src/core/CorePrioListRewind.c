// bdc 0x08a30690 CorePrioListRewind
#include "bdc.h"

/* Resets the list cursor to the first entry of the active chain (the node after the `active`
   sentinel). */
void CorePrioListRewind(CorePrioList *list)
{
    list->cursor = CorePrioNodeGetNext(list->active);
}
