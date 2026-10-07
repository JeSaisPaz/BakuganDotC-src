// bdc 0x089d8670 CoreObjectListClear
#include "bdc.h"

/* Deletes every object of a `CoreObject` list holder by running `CoreObjectChainDeleteAll` on
   its head (`*list`). */
void CoreObjectListClear(void *list)
{
    CoreObject **head = list;

    CoreObjectChainDeleteAll(*head);
}
