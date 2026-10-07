// bdc 0x089cf8bc NetCharaGetByIndex
#include "bdc.h"

/* Returns the `index`-th net-character object of `g_netCharaMgr` (list payload, node `+0xc`), or
   NULL when the manager is missing/empty or `index` is out of range. Takes the manager's
   `CoreLock` ("CONetChara SOLocal") around the list walk. Index 0 is the local character, which
   is always created first. */
NetChara *NetCharaGetByIndex(s32 index)
{
    NetCharaListNode *node;
    NetChara *result;
    s32 count;
    s32 i;

    result = NULL;
    if (g_netCharaMgr != NULL && g_netCharaMgr->list != NULL) {
        CoreLockAcquire(g_netCharaMgr->lock);
        count = g_netCharaMgr->list->count;
        if (count > 0 && index >= 0 && index < count) {
            node = NetCharaListFirst(g_netCharaMgr->list);
            result = NULL;
            i = 0;
            do {
                result = node->chara;
                i++;
                node = node->next;
            } while (i <= index);
        }
        CoreLockRelease(g_netCharaMgr->lock);
    }
    return result;
}
