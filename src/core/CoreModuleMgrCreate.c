// bdc 0x089cd75c CoreModuleMgrCreate
#include "bdc.h"

/* Allocates (low heap end) and constructs the 8-byte `COModule` manager with `CoreModuleMgrCtor`
   and stores it in `g_coreModuleMgr` (NULL on allocation failure). Called once by
   `BootDevModuleThread` at thread start. */
void CoreModuleMgrCreate(void)
{
    bool fromLow;
    void *mgr;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mgr = MemAlloc(8, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mgr != NULL)
        CoreModuleMgrCtor(mgr);
    g_coreModuleMgr = mgr;
}
