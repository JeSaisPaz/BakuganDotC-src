// bdc 0x08914214 UiUpgradeSetNumber
#include "bdc.h"

/* Shows a number with digit sprites on the Bakugan upgrade screen (task 490,
   `maybe_UiScreen490Ctor`; `"up_grade.fab"`, `"DMUpgrade"` texts): hides the 7 digit sprites
   ending at sprite index `lastSprite` (setting their `textureSlot = color`), splits `value` into
   digits (`UiNumberToDigits`) and shows them right-aligned from (`x`, `y`) with 11-pixel
   spacing, each digit's cell being (`d / 5`, `d % 5`). When `color` is non-zero, one more sprite
   left of the digits is shown with cell (0, 5). */

void UiUpgradeSetNumber(UiUpgrade *self, s32 value, s32 lastSprite, u8 color, float x, float y)
{
  GfxSprite **sprites;
  GfxSprite *sprite;
  int i;
  int digit;
  u8 count;
  u8 digits[8];

  memset(digits, 0, 8);
  for (i = 0; i < 7; i++) {
    sprites = (GfxSprite **)self->base.data;
    sprites[lastSprite - i]->flags &= ~1u;
    sprites = (GfxSprite **)self->base.data;
    sprites[lastSprite - i]->textureSlot = color;
  }
  UiNumberToDigits(digits, value, 7, 0xff);

  count = 0;
  if (digits[0] != 0xff) {
    do {
      count = count + 1;
    } while (digits[count] != 0xff);
  }

  for (i = 0; i < count; i++) {
    sprites = (GfxSprite **)self->base.data;
    sprites[lastSprite - i]->flags |= 1;
    sprites = (GfxSprite **)self->base.data;
    sprites[lastSprite - i]->posX = x - (float)i * 11.0f;
    sprites = (GfxSprite **)self->base.data;
    sprites[lastSprite - i]->posY = y;
    digit = digits[count - 1 - i];
    sprite = ((GfxSprite **)self->base.data)[lastSprite - i];
    GfxSpriteSetCell(sprite, (float)(digit / 5), (float)(digit % 5));
  }

  if (color != 0) {
    sprites = (GfxSprite **)self->base.data;
    sprites[lastSprite - i]->flags |= 1;
    sprites = (GfxSprite **)self->base.data;
    sprites[lastSprite - i]->posX = x - (float)i * 11.0f;
    sprites = (GfxSprite **)self->base.data;
    sprites[lastSprite - i]->posY = y;
    GfxSpriteSetCell(((GfxSprite **)self->base.data)[lastSprite - i], 0.0f, 5.0f);
  }
}
