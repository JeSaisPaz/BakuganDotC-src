// bdc 0x089edb28 GfxFaderIsReady
#include "bdc.h"

/* Returns 1 when the fader slot table `g_faderSlots` exists and its active slot (second word) is
   non-null, else 0. Guard used by `ScriptOpFade`. */

bool GfxFaderIsReady(void)

{

  
  bool ready = false;
  if ((g_faderSlots != (void **)0x0) && (g_faderSlots[1] != (void *)0x0)) {
    ready = true;
  }
  return ready;
}

