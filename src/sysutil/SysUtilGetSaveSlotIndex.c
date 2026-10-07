// bdc 0x089cc634 SysUtilGetSaveSlotIndex
#include "bdc.h"

/* Returns `g_saveSlotIndex`, the `BAKUGAN2%03d` slot last used by the savedata handler (-1 when
   unknown). `SaveAutoSaveTaskUpdate` falls back to the manual save list when it is negative. */

s32 SysUtilGetSaveSlotIndex(void)

{
  return g_saveSlotIndex;
}

