// bdc 0x089fb044 CoreMsCallback
#include "bdc.h"

/* Kernel callback "MyCB-MS" that the PSP calls when the Memory Stick is inserted or removed
   (`event` 1 = inserted, 2 = removed). On insert it sets `g_coreMsInserted` and
   `g_coreMsInsertEvent`; on removal it clears `g_coreMsInserted` and the `unk109` byte of
   `CoreMs`. Both paths also call `SysUtilResetSaveSlotIndex`; other events do nothing.
   Always returns 0. Registered by `CoreMsStateInit`; no direct callers. */
int CoreMsCallback(int arg1, int event, void *common)
{
    (void)arg1;
    (void)common;
    if (event == 1) {
        g_coreMsInserted = 1;
        g_coreMsInsertEvent = 1;
        SysUtilResetSaveSlotIndex();
    } else if (event == 2) {
        g_coreMsInserted = 0;
        CoreMsGet()->unk109 = 0;
        SysUtilResetSaveSlotIndex();
    }
    return 0;
}
