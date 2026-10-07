// bdc 0x089d6818 GfxMovieInitStep
#include "bdc.h"

/* Called each frame by `GfxMovieTaskUpdate` until it returns 1: advances the module loading
   (`GfxMovieLoadModulesStep`, step in `0x08ac5b8c`, stall counter `0x08ac5b90`) and, when it
   completes, sets the ready byte `0x08ac5b88` and creates the player (`GfxMovieCreatePlayer`). */

bool GfxMovieInitStep(void)
{
  s32 prev;
  bool ready = false;

  if (g_movieStateA == 0) {
    prev = g_movieStateB;
    g_movieStateB = GfxMovieLoadModulesStep(prev);
    if ((s32)g_movieStateB < 0) {
      g_movieStateA = 1;
      GfxMovieCreatePlayer();
    } else if ((s32)g_movieStateB == prev) {
      g_movieStateC = g_movieStateC + 1;
    } else {
      g_movieStateC = 0;
    }
  } else {
    ready = true;
  }
  return ready;
}
