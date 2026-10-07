// bdc 0x08948338 UiBattleRecordStartFade
#include "bdc.h"

/* Starts a full-screen fade of `frames` frames on the active fader from colour `from` to colour
   `to` (fader `start`/`end` RGBA quads copied whole, then `GfxFaderStart`). */

void UiBattleRecordStartFade(float frames, const float *from, const float *to)
{
  GfxFader *fader;
  s32 i;

  fader = GfxGetActiveFader();
  for (i = 0; i < 4; i++) {
    fader->start[i] = from[i];
  }
  fader = GfxGetActiveFader();
  for (i = 0; i < 4; i++) {
    fader->end[i] = to[i];
  }
  GfxFaderStart(GfxGetActiveFader(), (s32)frames);
}
