// bdc 0x08a2f98c SndBgmCmdListDestroy
#include "bdc.h"

/* Destructor of the BGM command list (`CoreList` copy): does nothing for NULL; empties the list
   (`SndBgmCmdListClear`), returns the sentinel to the pool (`MemPoolFree`) or frees it on the
   heap, destroys the pool (`MemPoolDestroy(pool, 3)`) and, when bit 0 of `flags` is set, frees the
   list object. Called with flags 3 by `SndBgmCmdListDestroyGlobal`. */
void SndBgmCmdListDestroy(CoreList *list, u32 flags)
{
    if (list == NULL) {
        return;
    }
    SndBgmCmdListClear(list);
    if (list->pool != NULL) {
        if (MemPoolFree(list->pool, list->sentinel)) {
            list->sentinel = NULL;
        }
        if (list->pool != NULL) {
            MemPoolDestroy(list->pool, 3);
            list->pool = NULL;
        }
    }
    if (list->sentinel != NULL) {
        CoreListNode *sentinel = list->sentinel;

        MemLock();
        MemFree(sentinel, NULL, 0);
        MemUnlock();
        list->sentinel = NULL;
    }
    if (flags & 1) {
        MemLock();
        MemFree(list, NULL, 0);
        MemUnlock();
    }
}
