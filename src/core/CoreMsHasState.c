// bdc 0x089fb00c CoreMsHasState
#include "bdc.h"

/* Returns 1 when the memory-stick service exists: `g_coreMsMgr` is set and its `CoreMs` block
   was created, else 0. No lock taken. */
int CoreMsHasState(void)
{
    if (g_coreMsMgr != NULL && g_coreMsMgr->ms != NULL) {
        return 1;
    }
    return 0;
}
