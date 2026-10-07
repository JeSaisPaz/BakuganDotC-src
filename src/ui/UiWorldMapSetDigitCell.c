// bdc 0x0899cf18 UiWorldMapSetDigitCell
#include "bdc.h"

/* Sets a digit/icon sprite of `UiWorldMap` to row `n` of its sheet
   (`GfxSpriteSetCell`, column 0), mapping 9 to 7. */

void UiWorldMapSetDigitCell(UiScreen *screen, GfxSprite *sprite, u8 n)
{
  if (n == 9) {
    n = 7;
  }
  GfxSpriteSetCell(sprite, 0.0f, (float)n);
}
