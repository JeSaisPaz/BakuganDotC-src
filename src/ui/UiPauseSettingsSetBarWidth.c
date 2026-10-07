// bdc 0x089acea0 UiPauseSettingsSetBarWidth
#include "bdc.h"

/* Sizes a volume bar sprite for `value` (0..10): width `5*value + 12` px, height 16 (UV rect and
   `UiSpriteSetSize`). */

void UiPauseSettingsSetBarWidth(UiPauseSettings *self, GfxSprite *bar, u8 value)
{
  float w = (float)(value * 5 + 12);
  float rect[4] = {0.0f, 0.0f, w, 16.0f};

  GfxSpriteSetUvRectXYWH(bar, rect);
  UiSpriteSetSize(w, 16.0f, bar);
}
