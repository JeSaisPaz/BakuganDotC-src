// bdc 0x089ad294 UiPauseSettingsGlowCursor
#include "bdc.h"

/* Applies the shared cursor glow (`UiCursorGlowStep`) to the sprite of the current item: plate
   0x1f+item for rows 0..3, button 0x2e+item for 4..6. */

void UiPauseSettingsGlowCursor(UiPauseSettings *self)

{
  int idx;
  s8 cur;

  cur = self->cursor;
  idx = 0x1f;
  if ((u32)(int)cur < 7) {
    switch (cur) {
    default:
      idx = cur + 0x1f;
      break;
    case 4:
    case 5:
    case 6:
      idx = cur + 0x2e;
    }
  }
  UiCursorGlowStep(((GfxSprite **)(self->base).data)[idx]);
  return;
}
