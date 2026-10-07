// bdc 0x0898caa8 UiCollectionFigureSetPageNumber
#include "bdc.h"

/* Shows the current page number (`page` + 1, up to 2 digits via `UiNumberToDigits`) of
   `UiCollectionFigure` in the digit sprites 0x21/0x22 (data `+0x84`,
   `+0x88`): hides both, then shows one sprite per digit (last digit in sprite 0x22) and picks
   its cell of the 5-column digit sheet with `GfxSpriteSetCell`. */

void UiCollectionFigureSetPageNumber(UiCollectionFigure *self)
{
    GfxSprite **sprites;
    u8 digits[4];
    u8 count = 0;
    int i;

    UiNumberToDigits(digits, self->page + 1, 2, 0xff);
    while (digits[count] != 0xff)
        count++;

    sprites = (GfxSprite **)self->base.data;
    sprites[0x21]->flags &= ~1u;
    sprites = (GfxSprite **)self->base.data;
    sprites[0x22]->flags &= ~1u;

    i = 0x22;
    do {
        u8 digit;

        sprites = (GfxSprite **)self->base.data;
        sprites[i]->flags |= 1;
        digit = digits[count - 1];
        sprites = (GfxSprite **)self->base.data;
        GfxSpriteSetCell(sprites[i], (float)(digit / 5), (float)(digit % 5));
        count--;
        i--;
    } while (count != 0 && i >= 0x21);
}
