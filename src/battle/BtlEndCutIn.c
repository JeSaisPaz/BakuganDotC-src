// bdc 0x0884df1c BtlEndCutIn
#include "bdc.h"

/* Removes the cut-in task 0x1e1 if present and then sets `g_btlHudHidden`. */
void BtlEndCutIn(void)
{
    if (CoreTaskExists(0x1e1) != 0) {
        CoreTaskRemoveById(0x1e1);
        g_btlHudHidden = 1;
    }
}
