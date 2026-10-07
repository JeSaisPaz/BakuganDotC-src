// bdc 0x089cdb88 CoreModuleUpdate
#include "bdc.h"

/* Step of the `COModule` module manager, called from the module thread. Under the manager lock it
   advances each of the 8 `g_coreModuleSlots`:
   - state 1 (load requested): slots 0..5 are utility modules, `sceUtilityLoadModule` of
     `CoreModuleGetUtilityId`; only the "already loaded" result `0x80111102` moves the slot to 3
     (running). Slots 6..7 are PRX files from the disc (`g_coreModulePrxPaths`), at most one per
     call: while the disc manager is held busy it loads the `IoMakePath` path with
     `sceKernelLoadModule` and on success stores the uid and moves to 2.
   - state 2: `sceKernelStartModule`; a positive result is stored as the id, state 3.
   - state 4 (unload requested): utility slots `sceUtilityUnloadModule` (success resets the slot and
     decrements `liveCount`); PRX slots without a uid drop to 0, else `sceKernelStopModule` → 5.
   - state 5: `sceKernelUnloadModule` (a positive result resets the slot and decrements
     `liveCount`); without a uid the slot drops to 0.
   When all 8 slots are in state 0 or 3 it releases the lock and sleeps the thread
   (`BootSleepCurrentThread`); otherwise it releases the lock and, if a disc load was attempted
   and did not succeed, waits for a vblank. */
void CoreModuleUpdate(CoreModuleMgr *mgr)
{
    char path[256];
    int idleCount = 0;
    bool discTried = false;
    int slot;

    CoreLockAcquire(mgr->lock);
    for (slot = 0; slot < 8; slot++) {
        CoreModuleSlot *entry = &g_coreModuleSlots[slot];
        u32 state = (u32)entry->state;
        int result;

        if (state >= 6) {
            continue;
        }
        switch (state) {
        case 1:
            if (slot < 6) {
                result = sceUtilityLoadModule(CoreModuleGetUtilityId(slot));
                if (result == (int)0x80111102) {
                    g_coreModuleSlots[slot].state = 3;
                }
            } else if (!discTried) {
                discTried = true;
                if (IoDiscHasManager() && IoDiscSetBusy(IoDiscGetManager(), true) != 0) {
                    SceUID uid;

                    IoMakePath(g_coreModulePrxPaths[slot - 6], path);
                    uid = sceKernelLoadModule(path, 0, NULL);
                    discTried = uid < 1;
                    if (!discTried) {
                        g_coreModuleSlots[slot].id = uid;
                        g_coreModuleSlots[slot].state = 2;
                    }
                    IoDiscSetBusy(IoDiscGetManager(), false);
                }
            }
            break;
        case 2:
            result = sceKernelStartModule(entry->id, 0, NULL, NULL, NULL);
            if (result > 0) {
                g_coreModuleSlots[slot].id = result;
                g_coreModuleSlots[slot].state = 3;
            }
            break;
        case 4:
            if (slot < 6) {
                if (sceUtilityUnloadModule(CoreModuleGetUtilityId(slot)) == 0) {
                    g_coreModuleSlots[slot].id = -1;
                    g_coreModuleSlots[slot].state = 0;
                    mgr->liveCount--;
                }
            } else if (entry->id < 1) {
                entry->state = 0;
            } else if (sceKernelStopModule(entry->id, 0, NULL, NULL, NULL) == 0) {
                g_coreModuleSlots[slot].state = 5;
            }
            break;
        case 5:
            if (entry->id < 1) {
                entry->state = 0;
            } else if (sceKernelUnloadModule(entry->id) > 0) {
                g_coreModuleSlots[slot].id = -1;
                g_coreModuleSlots[slot].state = 0;
                mgr->liveCount--;
            }
            break;
        default: /* 0 = unloaded, 3 = running */
            idleCount++;
            break;
        }
    }
    if (idleCount == 8) {
        CoreLockRelease(mgr->lock);
        BootSleepCurrentThread();
    } else {
        CoreLockRelease(mgr->lock);
        if (discTried) {
            sceDisplayWaitVblankStartCB();
        }
    }
}
