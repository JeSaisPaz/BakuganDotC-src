// bdc 0x089cd7fc CoreGetModuleMgr
#include "bdc.h"

/* Returns `g_coreModuleMgr` (singleton accessor, named by `bdc singleton`). */
void *CoreGetModuleMgr(void)
{
    return g_coreModuleMgr;
}
