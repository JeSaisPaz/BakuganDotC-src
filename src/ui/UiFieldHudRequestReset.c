// bdc 0x088cf008 UiFieldHudRequestReset
#include "bdc.h"

/* Sets `g_uiFieldHudResetRequest`; the next `UiFieldHudUpdate` then runs `UiFieldHudResetLayout`,
   re-enables the HUD and returns to phase 2. Caller: `GameFieldPhaseMain`. */

void UiFieldHudRequestReset(void)

{
  g_uiFieldHudResetRequest = 1;
  return;
}

