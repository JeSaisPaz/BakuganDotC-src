// bdc 0x0899d034 UiWorldMapSetStageScore
#include "bdc.h"

/* Shows `value` (up to 5 digits, `UiNumberToDigits`) with the five digit sprites of stage row `row` of
   `UiWorldMap`'s stage list (sprites `0x49 + row * 5`..): hides them, then shows
   one digit sprite per digit, right-aligned 12 px apart, relative to the panel origin
   (`panelPos` − `panelOffset[7]`, plus `y` on Y), each picking sheet cell (digit / 5, digit % 5). */

void UiWorldMapSetStageScore(float y, UiScreen *screen, u8 row, int value)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  u8 digits[6];
  float x;
  float digitY;
  int first = 0x49 + row * 5;
  int i;
  u8 count;
  u8 digit;
  GfxSprite *sprite;

  memset(digits, 0, 6);
  for (i = 0; i < 5; i++) {
    sprite = ((GfxSprite **)screen->data)[first + i];
    sprite->flags &= ~1u;
  }
  UiNumberToDigits(digits, value, 5, 0xff);

  count = 0;
  while (digits[count] != 0xff) {
    count++;
  }
  x = (map->panelPos[0] - map->panelOffset[7][0]) + (float)(count - 1) * 6.0f;
  digitY = (map->panelPos[1] - map->panelOffset[7][1]) + y;

  for (i = 0; i < count; i++) {
    sprite = ((GfxSprite **)screen->data)[first + i];
    sprite->flags |= 1;
    ((GfxSprite **)screen->data)[first + i]->posX = x - (float)i * 12.0f;
    ((GfxSprite **)screen->data)[first + i]->posY = digitY;
    digit = digits[count - 1 - i];
    GfxSpriteSetCell(((GfxSprite **)screen->data)[first + i], (float)(digit / 5), (float)(digit % 5));
  }
}
