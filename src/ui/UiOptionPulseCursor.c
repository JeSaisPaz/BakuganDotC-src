// bdc 0x089714c4 UiOptionPulseCursor
#include "bdc.h"

/* Animates (`UiCursorGlowStep`) the sprite under the cursor of `UiOption`: the row
   plate (0x1f + row) for rows 0..3, the button (0x2c + row) for rows 4/5. */

void UiOptionPulseCursor(UiOption *self)
{
    int row = (s8)self->cursor;
    GfxSprite **sprites = (GfxSprite **)self->base.data;
    GfxSprite *sprite;

    if (row < 0 || row > 3) {
        sprite = sprites[row + 0x2c];
    } else {
        sprite = sprites[row + 0x1f];
    }
    UiCursorGlowStep(sprite);
}
