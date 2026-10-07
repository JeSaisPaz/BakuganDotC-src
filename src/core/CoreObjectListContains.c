// bdc 0x089d86c4 CoreObjectListContains
#include "bdc.h"

/* Returns 1 when `obj` is linked in the chain whose first object is `*head`, else 0 (wrapper over
   `CoreObjectChainContains`). */
s32 CoreObjectListContains(CoreObject **head, CoreObject *obj)
{
    return CoreObjectChainContains(*head, obj);
}
