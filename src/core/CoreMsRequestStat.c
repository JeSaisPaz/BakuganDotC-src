// bdc 0x089fb164 CoreMsRequestStat
#include "bdc.h"

/* Queues a probe of `path` on the memory stick, to be performed by the next `CoreMsUpdate`: under
   the "COMS::Create" lock, if no stat and no mkdir request is pending, it sets `statPending`,
   clears `pathExists` and copies `path` into `CoreMs``.path` (`strcpy`, no length check against
   the 0x100-byte buffer). With `path == NULL` it instead raises `resetRequest` and clears
   `unk109`/`unk10a` so that the update drops all pending state. Returns 1 when the request was
   accepted, 0 when another request was still pending. */
int CoreMsRequestStat(CoreMs *ms, const char *path)
{
    int accepted = 0;

    CoreLockAcquire(g_coreMsMgr->lock);
    if (!ms->statPending && !ms->mkdirPending) {
        ms->statPending = 1;
        accepted = 1;
        ms->pathExists = 0;
        if (path == NULL) {
            ms->resetRequest = 1;
            ms->unk109 = 0;
            ms->unk10a = 0;
        } else {
            strcpy(ms->path, path);
        }
    }
    CoreLockRelease(g_coreMsMgr->lock);
    return accepted;
}
