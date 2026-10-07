// bdc 0x0885b67c BtlLoadRequestCreate
#include "bdc.h"

/* Creates a Bakugan load request: under `MemLock`, allocates the 0x34-byte object from the low
   end of the heap (restoring the previous placement policy afterwards), constructs it with
   `BtlLoadRequestCtor``(obj, kind)` when the allocation succeeded, makes sure the request list
   `g_btlLoadRequests` exists (`BtlLoadRequestListEnsure`) and appends the request to it
   (`CoreObjectListAppend`, called with NULL too when the allocation failed). Returns the
   request or NULL. */
void *BtlLoadRequestCreate(u32 kind)
{
    bool fromLow;
    BtlLoadRequest *mem;
    BtlLoadRequest *req = NULL;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(0x34, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mem != NULL) {
        BtlLoadRequestCtor(mem, kind);
        req = mem;
    }
    if (g_btlLoadRequests == NULL) {
        BtlLoadRequestListEnsure();
    }
    CoreObjectListAppend((CoreObject *)req, g_btlLoadRequests);
    return req;
}
