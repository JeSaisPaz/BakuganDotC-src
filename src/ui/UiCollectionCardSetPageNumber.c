// bdc 0x089838f4 UiCollectionCardSetPageNumber
#include "bdc.h"

/* Shows the current page number (`page` + 1) of `UiCollectionCard` in the
   digit sprites 0x1d/0x1e (`UiNumberToDigits`, cells by digit: column digit / 5, row digit % 5):
   hides both, then shows one sprite per digit from 0x1e down, last digit first. */

void UiCollectionCardSetPageNumber(UiCollectionCard *self)

{
  u8 digits[4];
  u8 count = 0;
  s32 i;
  GfxSprite *sprite;

  UiNumberToDigits(digits, self->page + 1, 2, 0xff);
  sprite = ((GfxSprite **)self->base.data)[0x1d];
  if (digits[0] != 0xff) {
    count = 1;
    while (digits[count] != 0xff) {
      count++;
    }
  }
  sprite->flags &= ~1u;
  ((GfxSprite **)self->base.data)[0x1e]->flags &= ~1u;
  i = 0x1e;
  do {
    ((GfxSprite **)self->base.data)[i]->flags |= 1;
    GfxSpriteSetCell(((GfxSprite **)self->base.data)[i], (float)(digits[count - 1] / 5),
                     (float)(digits[count - 1] % 5));
    count--;
    if (count == 0) {
      return;
    }
    i--;
  } while (i >= 0x1d);
}
