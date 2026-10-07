// bdc 0x0890a568 UiGetMenuResult
#include "bdc.h"

/* Returns the last menu result stored with `UiSetMenuResult` (word `+0xc` of the object at
   `g_scriptGlobalVars`). Parent screens poll it after the child screen task is gone. */

s32 UiGetMenuResult(UiScreen *screen)

{
  return g_scriptGlobalVars[3];
}

