// bdc 0x089d89e0 CoreObjectChainContains
#include "bdc.h"

/* Walks the `next` chain from `first` and returns 1 as soon as a node equals `obj`, 0 at the end
   of the chain. */
s32 CoreObjectChainContains(CoreObject *first, CoreObject *obj)
{
    for (; first != NULL; first = first->next) {
        if (first == obj) {
            return 1;
        }
    }
    return 0;
}
