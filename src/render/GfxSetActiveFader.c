// bdc 0x089edb60 GfxSetActiveFader
#include "bdc.h"

/* Makes `fader` the active screen fade overlay in `g_faderSlots` (NULL restores the default
   fader) and returns the previously active fader, or NULL if the default was active. Does nothing
   and returns NULL if the fader slots were never initialised. */

GfxFader * GfxSetActiveFader(GfxFader *fader)
{
  GfxFader *prev = (GfxFader *)0;
  if (g_faderSlots != (void **)0) {
    if (g_faderSlots[1] != *g_faderSlots) {
      prev = g_faderSlots[1];
    }
    if (fader == (GfxFader *)0) {
      g_faderSlots[1] = *g_faderSlots;
      return prev;
    }
    g_faderSlots[1] = fader;
  }
  return prev;
}
