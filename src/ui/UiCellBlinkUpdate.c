// bdc 0x0892c140 UiCellBlinkUpdate
#include "bdc.h"

/* Steps a cell-blink record: every 16 frames (8 with frame skip) toggles the phase and switches the
   sprite's UV rectangle between (0, 0, 256, 16) and (128, 0, 256, 16). */

void UiCellBlinkUpdate(UiCellBlink *rec)

{
  u8 phase;
  float limit;
  float rectA[4];
  float rectB[4];

  if (rec->on != 0) {
    phase = rec->phase;
    if (g_gfxDisplay->frameSkip == 0) {
      limit = 16.0f;
    }
    else {
      limit = 8.0f;
    }
    if (rec->t == limit) {
      rec->t = 0.0f;
      rec->phase = phase ^ 1;
      phase = rec->phase;
    }
    else {
      rec->t = rec->t + 1.0f;
    }
    if (phase == 0) {
      rectA[0] = 0.0f;
      rectA[1] = 0.0f;
      rectA[2] = 256.0f;
      rectA[3] = 16.0f;
      GfxSpriteSetUvRectXYWH(rec->sprite, rectA);
      return;
    }
    rectB[1] = 0.0f;
    rectB[0] = 128.0f;
    rectB[2] = 256.0f;
    rectB[3] = 16.0f;
    GfxSpriteSetUvRectXYWH(rec->sprite, rectB);
  }
  return;
}
