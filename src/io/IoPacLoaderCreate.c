// bdc 0x089fd748 IoPacLoaderCreate
#include "bdc.h"

/* Creates the `.pac` package loader singleton `g_ioPacLoader` if it does not exist (8 bytes from
   the low heap, `IoPacLoaderCtor`). Called from `IoPacLoaderTaskCtor`. */
void IoPacLoaderCreate(void)
{
    bool fromLow;
    void *proc;

    if (g_ioPacLoader != NULL) {
        return;
    }
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    proc = MemAlloc(sizeof(IoPacLoader), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (proc != NULL) {
        IoPacLoaderCtor(proc);
    }
    g_ioPacLoader = proc;
}
