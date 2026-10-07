// bdc 0x089cf304 NetCharaMgrCreate
#include "bdc.h"

/* Creates the `CONetChara` manager `g_netCharaMgr` if missing (0x94 zeroed bytes from the low
   heap: LwMutex `CoreLock` "CONetChara SOLocal" `lock`, 16-entry `NetCharaListInit` `list`
   with its `iterating` byte set to 1), then, under that lock, allocates the zeroed 0x3c0-byte
   lock-step frame buffer `g_netCharaSlots` if missing. Always finishes by creating the invite
   object (`NetInviteCreate`) and setting `g_netCharaMgrActive`. */

void NetCharaMgrCreate(void)
{
    bool fromLow;
    NetCharaMgr *mgr;
    CoreLock *lock;
    NetCharaList *list;
    void *slots;

    if (g_netCharaMgr == NULL) {
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        mgr = MemAlloc(sizeof(NetCharaMgr), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        g_netCharaMgr = mgr;
        memset(mgr, 0, sizeof(NetCharaMgr));

        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        lock = MemAlloc(sizeof(CoreLock), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (lock != NULL) {
            CoreLockInit(lock, "CONetChara SOLocal", CORE_LOCK_LWMUTEX);
        }
        g_netCharaMgr->lock = lock;

        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        list = MemAlloc(sizeof(NetCharaList), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (list != NULL) {
            NetCharaListInit(list, 0x10);
        }
        mgr = g_netCharaMgr;
        mgr->list = list;
        list->iterating = 1;

        CoreLockAcquire(mgr->lock);
        if (g_netCharaSlots == NULL) {
            MemLock();
            fromLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            slots = MemAlloc(12 * 2 * sizeof(NetCharaMsg), NULL, 0);
            MemSetAllocFromLow(fromLow);
            MemUnlock();
            memset(slots, 0, 12 * 2 * sizeof(NetCharaMsg));
            g_netCharaSlots = slots;
        }
        CoreLockRelease(g_netCharaMgr->lock);
    }
    NetInviteCreate();
    g_netCharaMgrActive = 1;
}
