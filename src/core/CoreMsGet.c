// bdc 0x089fb034 CoreMsGet
#include "bdc.h"

/* Returns the `CoreMs` request block of the memory-stick service (`g_coreMsMgr->ms`). No NULL
   check: use `CoreMsHasState` first when the service may not exist. `main` passes it to
   `CoreMsUpdate` every frame. */
CoreMs *CoreMsGet(void)
{
    return g_coreMsMgr->ms;
}
