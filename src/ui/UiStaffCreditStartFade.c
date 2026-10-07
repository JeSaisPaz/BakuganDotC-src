// bdc 0x08944550 UiStaffCreditStartFade
#include "bdc.h"

/* Starts a full-screen fade of `frames` frames on the active fader from colour `from` to colour
   `to` (RGBA floats copied to fader `start` and `end`, `GfxFaderStart`). */

void UiStaffCreditStartFade(float frames, const float *from, const float *to)

{
  GfxFader *fader;
  float r, g, b, a;

  fader = GfxGetActiveFader();
  r = from[0];
  g = from[1];
  b = from[2];
  a = from[3];
  fader->start[0] = r;
  fader->start[1] = g;
  fader->start[2] = b;
  fader->start[3] = a;
  fader = GfxGetActiveFader();
  r = to[0];
  g = to[1];
  b = to[2];
  a = to[3];
  fader->end[0] = r;
  fader->end[1] = g;
  fader->end[2] = b;
  fader->end[3] = a;
  fader = GfxGetActiveFader();
  GfxFaderStart(fader, (s32)frames);
}
