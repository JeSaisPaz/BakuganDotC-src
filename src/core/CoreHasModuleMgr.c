// bdc 0x089cd7e0 CoreHasModuleMgr
#include "bdc.h"

/* Returns whether `g_coreModuleMgr` is non-null (singleton accessor, named by `bdc singleton`). */
bool CoreHasModuleMgr(void)
{
    return g_coreModuleMgr != NULL;
}
