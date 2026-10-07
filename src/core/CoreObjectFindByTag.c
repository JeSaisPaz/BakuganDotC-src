// bdc 0x089d89ac CoreObjectFindByTag
#include "bdc.h"

/* Walks a `CoreObject` chain from `first` and returns the first object whose `unk08` (`+0x8`)
   equals `tag`, or NULL. */
CoreObject *CoreObjectFindByTag(CoreObject *first, u32 tag)
{
    for (; first != NULL; first = first->next) {
        if (first->unk08 == tag) {
            return first;
        }
    }
    return NULL;
}
