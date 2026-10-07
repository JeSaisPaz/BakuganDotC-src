// bdc 0x089cc624 SysUtilResetSaveSlotIndex
#include "bdc.h"

/* Sets `g_saveSlotIndex` to -1 (no known save slot). Called by `SaveProfileReset` and
   `CoreMsCallback` (memory stick removed/changed). */

void SysUtilResetSaveSlotIndex(void)

{
  g_saveSlotIndex = -1;
  return;
}

