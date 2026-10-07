// bdc 0x089d8a6c CoreObjectClearLinks
#include "bdc.h"

/* Zeroes the first five header words of a `CoreObject` (`prev`, `next`, `unk08`, `id`, owner
   `list`). */
void CoreObjectClearLinks(CoreObject *obj)
{
    obj->next = NULL;
    obj->prev = NULL;
    obj->list = NULL;
    obj->id = 0;
    obj->unk08 = 0;
}
