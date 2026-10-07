// bdc 0x089d907c CoreNodeOwnerCtor
#include "bdc.h"

/* Constructor of `CoreNodeOwner`: runs `CoreNodeCtor` (no anchor), installs
   `g_coreNodeOwnerVtbl` and clears the list head, tail and `unk2c`. Returns `owner`. */
CoreNodeOwner *CoreNodeOwnerCtor(CoreNodeOwner *owner)
{
    CoreNodeCtor((CoreNode *)owner, NULL);
    owner->vtable = g_coreNodeOwnerVtbl;
    owner->head = NULL;
    owner->tail = NULL;
    owner->unk2c = 0;
    return owner;
}
