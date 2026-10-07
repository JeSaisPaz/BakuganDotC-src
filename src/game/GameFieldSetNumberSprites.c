// bdc 0x088be104 GameFieldSetNumberSprites
#include "bdc.h"

/* Shows `value` (clamped at 0) with two digit sprites at y 122: picks the digit cells from a
   5-column sheet (`GfxSpriteSetCell`), places the ones digit at x 305 alone or at 329 after the
   tens digit (x 305) when `value` ≥ 10, sets flag bit 0 of the ones sprite, and sets flag bit 0 of
   the tens sprite when `value` ≥ 10 or clears it (hides the tens digit) below 10. */

void GameFieldSetNumberSprites(GfxSprite *tens, GfxSprite *ones, s32 value)

{
  if (value < 0) {
    value = 0;
  }
  if (value < 10) {
    tens->posX = 305.0f;
    ones->posX = 305.0f;
  }
  else {
    tens->posX = 305.0f;
    ones->posX = 329.0f;
  }
  tens->posY = 122.0f;
  ones->posY = 122.0f;
  GfxSpriteSetCell(tens, (float)((value / 10) / 5), (float)((value / 10) % 5));
  GfxSpriteSetCell(ones, (float)((value % 10) / 5), (float)((value % 10) % 5));
  ones->flags |= 1;
  if (value < 10) {
    tens->flags &= ~1u;
    return;
  }
  tens->flags |= 1;
}
