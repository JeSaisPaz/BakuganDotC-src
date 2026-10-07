// bdc 0x089fb208 CoreMsIsIdle
#include "bdc.h"

/* Returns 1 when no stat request is pending on the memory-stick mailbox (`ms->statPending == 0`),
   read under the "COMS::Create" lock. The file loader waits for it before continuing a read that
   targets the Memory Stick. */
int CoreMsIsIdle(CoreMs *ms)
{
    int idle;

    CoreLockAcquire(g_coreMsMgr->lock);
    idle = ms->statPending == 0;
    CoreLockRelease(g_coreMsMgr->lock);
    return idle;
}
