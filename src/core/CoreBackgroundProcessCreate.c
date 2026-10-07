// bdc 0x089ff73c CoreBackgroundProcessCreate
#include "bdc.h"

/* Creates the `COBackGroundProcess` worker singleton `g_coreBackgroundProcess` if it does not
   exist (8 bytes from the low heap, `CoreBackgroundProcessCtor`); stores NULL when the
   allocation fails. */
void CoreBackgroundProcessCreate(void)
{
    bool fromLow;
    void *proc;

    if (g_coreBackgroundProcess != NULL) {
        return;
    }
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    proc = MemAlloc(8, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (proc != NULL) {
        CoreBackgroundProcessCtor(proc);
    }
    g_coreBackgroundProcess = proc;
}
