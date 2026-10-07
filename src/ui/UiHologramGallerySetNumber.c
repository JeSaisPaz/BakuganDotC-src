// bdc 0x0891fd30 UiHologramGallerySetNumber
#include "bdc.h"

/* Shows a number with digit sprites on the hologram gallery screen (task 391,
   `maybe_UiScreen391Ctor`, class prefix `UiHologramGallery`): hides the 7 digit sprites ending at
   sprite index `lastSprite` (setting their textureSlot = `color`), splits `value` into digits
   (`UiNumberToDigits`) and shows them right-aligned from (`x`, `y`) with 11-pixel spacing, each
   digit's cell being (`d / 5`, `d % 5`). When `color` is nonzero, one more sprite left of the
   digits shows cell (0, 5). */

void UiHologramGallerySetNumber(UiHologramGallery *self, s32 value, s32 lastSprite, u8 color, float x, float y)
{
  GfxSprite **sprites;
  GfxSprite *sprite;
  u8 digits[8];
  u8 count;
  int i;
  int d;

  memset(digits, 0, 8);
  for (i = 0; i < 7; i++) {
    sprites = (GfxSprite **)self->base.data;
    sprites[lastSprite - i]->flags &= ~1u;
    sprites = (GfxSprite **)self->base.data;
    sprites[lastSprite - i]->textureSlot = color;
  }
  UiNumberToDigits(digits, value, 7, 0xff);
  count = 0;
  while (digits[count] != 0xff) {
    count++;
  }
  for (i = 0; i < count; i++) {
    sprite = ((GfxSprite **)self->base.data)[lastSprite - i];
    sprite->flags |= 1;
    ((GfxSprite **)self->base.data)[lastSprite - i]->posX = x - (float)i * 11.0f;
    ((GfxSprite **)self->base.data)[lastSprite - i]->posY = y;
    d = digits[count - 1 - i];
    GfxSpriteSetCell(((GfxSprite **)self->base.data)[lastSprite - i], (float)(d / 5), (float)(d % 5));
  }
  if (color != 0) {
    sprite = ((GfxSprite **)self->base.data)[lastSprite - i];
    sprite->flags |= 1;
    ((GfxSprite **)self->base.data)[lastSprite - i]->posX = x - (float)i * 11.0f;
    ((GfxSprite **)self->base.data)[lastSprite - i]->posY = y;
    GfxSpriteSetCell(((GfxSprite **)self->base.data)[lastSprite - i], 0.0f, 5.0f);
  }
}
