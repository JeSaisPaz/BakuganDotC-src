// bdc 0x089cf974 NetCharaFindByMac
#include "bdc.h"

/* Returns the net character whose MAC (`outHdr.mac`, 6 bytes) equals `mac`, searching the
   `g_netCharaMgr` list under its lock; NULL when not found. */
NetChara *NetCharaFindByMac(u8 *mac)
{
    NetCharaListNode *node;
    NetChara *result;
    s32 count;
    s32 i;
    s32 j;

    result = NULL;
    if (g_netCharaMgr != NULL && g_netCharaMgr->list != NULL) {
        CoreLockAcquire(g_netCharaMgr->lock);
        count = g_netCharaMgr->list->count;
        if (count > 0) {
            node = NetCharaListFirst(g_netCharaMgr->list);
            for (i = 0; i < count; i++) {
                result = node->chara;
                if (result != NULL) {
                    for (j = 0; j < 6; j++) {
                        if (result->outHdr.mac[j] != mac[j]) {
                            result = NULL;
                            break;
                        }
                    }
                    if (result != NULL) {
                        break;
                    }
                }
                node = node->next;
            }
        }
        CoreLockRelease(g_netCharaMgr->lock);
    }
    return result;
}
