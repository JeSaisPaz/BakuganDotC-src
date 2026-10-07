// bdc 0x089d8a0c CoreObjectFindById
#include "bdc.h"

/* Walks a `CoreObject` sibling chain starting at `first` through `next` and returns the first
   object whose `id` equals `id`, or NULL. */
CoreObject *CoreObjectFindById(CoreObject *first, u32 id)
{
    CoreObject *obj;

    for (obj = first; obj != NULL; obj = obj->next) {
        if (obj->id == id)
            return obj;
    }
    return NULL;
}
