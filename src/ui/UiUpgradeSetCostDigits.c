// bdc 0x08914698 UiUpgradeSetCostDigits
#include "bdc.h"

/* Shows `value` with up to 5 digit sprites ending at sprite index `lastSprite` of the Bakugan
   upgrade screen (`UiUpgradeCtor`, task 490): hides the five sprites, splits the value into
   digits (`UiNumberToDigits`) and shows them right-aligned from (`x`, `y`) with 10-pixel spacing,
   cell (`d / 5`, `d % 5`). */

void UiUpgradeSetCostDigits(float x, float y, UiUpgrade *self, int value, int lastSprite)
{
  GfxSprite **sprites;
  u8 digits[16];
  u8 count;
  u8 digit;
  int i;

  memset(digits, 0, 6);
  for (i = 0; i < 5; i++) {
    sprites = (GfxSprite **)self->base.data;
    sprites[lastSprite - i]->flags &= ~1u;
  }
  UiNumberToDigits(digits, (u32)value, 5, 0xff);
  count = 0;
  if (digits[0] != 0xff) {
    do {
      count++;
    } while (digits[count] != 0xff);
  }
  for (i = 0; i < count; i++) {
    ((GfxSprite **)self->base.data)[lastSprite - i]->flags |= 1;
    ((GfxSprite **)self->base.data)[lastSprite - i]->posX = x - (float)i * 10.0f;
    ((GfxSprite **)self->base.data)[lastSprite - i]->posY = y;
    digit = digits[count - 1 - i];
    GfxSpriteSetCell(((GfxSprite **)self->base.data)[lastSprite - i], (float)(digit / 5),
                     (float)(digit % 5));
  }
}
