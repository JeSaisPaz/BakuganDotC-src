// bdc 0x089f941c IoDiscHasManager
#include "bdc.h"

/* Returns whether the disc-access manager `g_discSimple` exists (`g_discSimple != NULL`). Seven
   callers, including `NetPlayUpdate`-side code (`NetPlayLateUpdate`). */
bool IoDiscHasManager(void)
{
    return g_discSimple != NULL;
}
