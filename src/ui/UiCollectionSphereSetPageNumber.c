// bdc 0x0897a908 UiCollectionSphereSetPageNumber
#include "bdc.h"

/* Shows the current page number (`page` + 1) of `UiCollectionSphere` in
   the digit sprites 0x23/0x24 (`UiNumberToDigits`, cells by digit): hides both, then shows one
   sprite per digit from 0x24 down, last digit first. */

void UiCollectionSphereSetPageNumber(UiCollectionSphere *self)

{
  u8 digits[4];
  u8 count = 0;
  s32 i;
  GfxSprite *sprite;

  UiNumberToDigits(digits, self->page + 1, 2, 0xff);
  sprite = ((GfxSprite **)self->base.data)[0x23];
  if (digits[0] != 0xff) {
    count = 1;
    while (digits[count] != 0xff) {
      count++;
    }
  }
  sprite->flags &= ~1u;
  ((GfxSprite **)self->base.data)[0x24]->flags &= ~1u;
  i = 0x24;
  do {
    ((GfxSprite **)self->base.data)[i]->flags |= 1;
    GfxSpriteSetCell(((GfxSprite **)self->base.data)[i], (float)(digits[count - 1] / 5),
                     (float)(digits[count - 1] % 5));
    count--;
    if (count == 0) {
      return;
    }
    i--;
  } while (i >= 0x23);
}
