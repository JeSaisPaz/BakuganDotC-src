// bdc 0x089fb268 CoreMsUpdate
#include "bdc.h"

/* Per-frame service of the memory-stick mailbox, called by `main`'s service loop (only while
   `CorePowerIsRunning`). First turns a pending insert event (`g_coreMsInsertEvent`) into a
   reset request (`CoreMsRequestStat``(ms, NULL)`). Then, under the "COMS::Create" lock: with no
   stick inserted (`g_coreMsInserted` == 0) it clears the capacity record; with one inserted and
   no capacity yet it asks the driver (`sceIoDevctl("ms0:", 0x02425818, &devSizePtr, 4, NULL, 0)`)
   and, on success, marks `capacityValid` and resets `writeProtect` to -1. It then executes one
   request: a reset clears `pathExists`/`statPending`; a stat request runs
   `sceIoGetstat(path, &mgr->stat)` and sets `pathExists` on success; a mkdir request runs
   `sceIoMkdir(path, 0)` and sets `mkdirDone` on success. Finally, while `writeProtect < 0` (and a
   stick with known capacity is present) it reads the write-protect state (`sceIoDevctl("ms0:",
   0x02425824, NULL, 0, &out, 4)`) into `writeProtect` (the negative error code on failure) and
   `writeProtected`. */
void CoreMsUpdate(CoreMs *ms)
{
    s32 result;
    s32 out;

    if (g_coreMsInsertEvent != 0) {
        CoreMsRequestStat(ms, NULL);
        g_coreMsInsertEvent = 0;
    }
    CoreLockAcquire(g_coreMsMgr->lock);

    if (g_coreMsInserted == 0) {
        if (g_coreMsMgr->capacityValid != 0) {
            memset(&g_coreMsMgr->devSize, 0, 0x14);
            g_coreMsMgr->capacityValid = 0;
        }
    } else if (g_coreMsMgr->capacityValid == 0) {
        if (sceIoDevctl("ms0:", 0x02425818, &g_coreMsMgr->devSizePtr, 4, NULL, 0) == 0) {
            g_coreMsMgr->capacityValid = 1;
            ms->writeProtect = -1;
        }
    }

    if (ms->resetRequest != 0) {
        ms->pathExists = 0;
        ms->statPending = 0;
        ms->resetRequest = 0;
    } else if (ms->statPending != 0) {
        if (sceIoGetstat(ms->path, &g_coreMsMgr->stat) == 0) {
            ms->pathExists = 1;
        }
        ms->statPending = 0;
    } else if (ms->mkdirPending != 0) {
        if (sceIoMkdir(ms->path, 0) == 0) {
            ms->mkdirDone = 1;
        }
        ms->mkdirPending = 0;
    }

    if (g_coreMsInserted != 0 && g_coreMsMgr->capacityValid != 0 && ms->writeProtect < 0) {
        result = sceIoDevctl("ms0:", 0x02425824, NULL, 0, &out, 4);
        if (result >= 0) {
            result = out;
        }
        ms->writeProtect = result;
        ms->writeProtected = (result != 0) ? 1 : 0;
    }

    CoreLockRelease(g_coreMsMgr->lock);
}
