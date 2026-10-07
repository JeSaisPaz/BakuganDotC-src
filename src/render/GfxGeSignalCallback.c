// bdc 0x089ce858 GfxGeSignalCallback
#include "bdc.h"

/* GE signal callback installed with `sceGuSetCallback`(1, ...) by `GfxDisplaySetup`: calls the
   optional `GfxDisplay` `signalHook` (`g_gfxDisplay+0x14`) and then `sceGeContinue()` to resume
   the stalled list. */

void GfxGeSignalCallback(void)

{
  if (g_gfxDisplay->signalHook != 0) {
    (*g_gfxDisplay->signalHook)();
  }
  sceGeContinue();
  return;
}

