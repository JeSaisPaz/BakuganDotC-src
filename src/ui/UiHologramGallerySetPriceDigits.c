// bdc 0x0891d2a4 UiHologramGallerySetPriceDigits
#include "bdc.h"

/* Shows a price with digit sprites on the hologram gallery screen (task 391,
   `UiHologramGalleryCtor`): hides the four digit sprites `lastSprite`, `lastSprite - 1`, ... and
   tints them red when the available points of the selected slot
   (`UiHologramGalleryGetSlotCost` `points`) are below `price`, white otherwise; then splits
   `price` into up to 4 digits (`UiNumberToDigits`), shows one sprite per digit, last digit
   first, right-aligned from `x + (digits - 1) * 6` at 12-pixel steps leftwards on row `y`, with
   sprite-sheet cell (`digit / 5`, `digit % 5`) (`GfxSpriteSetCell`). */

void UiHologramGallerySetPriceDigits(UiHologramGallery *self, s32 price, s32 lastSprite, float x, float y)
{
  u8 digits[5];
  HologramSlotCost cost;
  GfxSprite *sprite;
  u8 count;
  u8 digit;
  int i;

  memset(digits, 0, 5);
  UiHologramGalleryGetSlotCost(&cost, self, 0, 0);
  for (i = 0; i < 4; i++) {
    sprite = ((GfxSprite **)self->base.data)[lastSprite - i];
    sprite->flags &= ~1u;
    sprite = ((GfxSprite **)self->base.data)[lastSprite - i];
    if (cost.points < price) {
      sprite->tint[0] = 1.0f;
      sprite->tint[1] = 0.0f;
      sprite->tint[2] = 0.0f;
      sprite->alpha = 1.0f;
    } else {
      sprite->tint[0] = 1.0f;
      sprite->tint[1] = 1.0f;
      sprite->tint[2] = 1.0f;
      sprite->alpha = 1.0f;
    }
  }

  UiNumberToDigits(digits, price, 4, 0xff);
  count = 0;
  if (digits[0] != 0xff) {
    do {
      count++;
    } while (digits[count] != 0xff);
  }

  x = x + (float)(count - 1) * 6.0f;
  for (i = 0; i < count; i++) {
    sprite = ((GfxSprite **)self->base.data)[lastSprite - i];
    sprite->flags |= 1;
    ((GfxSprite **)self->base.data)[lastSprite - i]->posX = x - (float)i * 12.0f;
    ((GfxSprite **)self->base.data)[lastSprite - i]->posY = y;
    digit = digits[count - 1 - i];
    GfxSpriteSetCell(((GfxSprite **)self->base.data)[lastSprite - i], (float)(digit / 5),
                     (float)(digit % 5));
  }
}
