// bdc 0x089bb340 BootIsExitRequested
#include "bdc.h"

/* Returns the exit-request byte `g_bootExitRequested`. `main` polls it every frame of its
   service loop (together with `CorePowerIsSuspended`) and `BootMainThread` spins on it at the
   end of each frame (`while (BootIsExitRequested()) sceDisplayWaitVblankStartCB();`), which holds
   the game thread before `BootEndOfFrame` while the byte is set. */

u8 BootIsExitRequested(void)

{
  return g_bootExitRequested;
}

