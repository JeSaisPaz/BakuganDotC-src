// bdc 0x0890a558 UiSetMenuResult
#include "bdc.h"

/* Stores a screen's menu result in word `+0xc` (index 3) of `g_scriptGlobalVars`, where the parent
   screen or game flow reads it back with `UiGetMenuResult` once the child screen closes (e.g.
   `UiBattleModeSelect` stores 1/2/0; the pause menu's results 0xe/0xf lead
   to `UiPauseSettings` / task 10020). The first argument (the screen) is
   unused. */

void UiSetMenuResult(UiScreen *screen, s32 result)

{
  g_scriptGlobalVars[3] = result;
  return;
}

