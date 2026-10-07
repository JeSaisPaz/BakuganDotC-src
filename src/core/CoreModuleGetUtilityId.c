// bdc 0x089cd730 CoreModuleGetUtilityId
#include "bdc.h"

/* Maps module slot `slot` 0..5 to its `PSP_MODULE_*` utility id from `g_coreModuleUtilityIds`;
   other values are returned unchanged. Used by `CoreModuleUpdate` for
   `sceUtilityLoadModule`/`sceUtilityUnloadModule`. */
int CoreModuleGetUtilityId(int slot)
{
    if (slot >= 0 && slot < 6)
        return g_coreModuleUtilityIds[slot];
    return slot;
}
