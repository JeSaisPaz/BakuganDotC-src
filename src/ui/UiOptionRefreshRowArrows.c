// bdc 0x089710c4 UiOptionRefreshRowArrows
#include "bdc.h"

/* Colours the left/right arrow sprites of option row `row` of the battle-options screen (task 304,
   `maybe_UiScreen304Ctor`; `"option_battle_t_%02d"` / `"option_sol00"` sprites; edits the battle
   rules stored in profile words 0x18..0x1b) (sprites `row*3+1` and `row*3+13`): white normally,
   50 % grey on the side where the value is already at its minimum or maximum. Row 0 greys both
   arrows when `SaveGetProfileFlag0` is set, row 3 when its value is -1; rows above 3 only get
   the white reset. */

void UiOptionRefreshRowArrows(UiOption *self, u8 row)
{
    int left = row * 3 + 1;
    int right = row * 3 + 13;
    GfxSprite *s;
    int i;

    s = ((GfxSprite **)self->base.data)[left];
    for (i = 0; i < 3; i++) {
        s->tint[i] = 1.0f;
    }
    s->alpha = 1.0f;
    s = ((GfxSprite **)self->base.data)[right];
    for (i = 0; i < 3; i++) {
        s->tint[i] = 1.0f;
    }
    s->alpha = 1.0f;

    if (row > 3) {
        return;
    }
    if ((row == 0 && SaveGetProfileFlag0() != 0) || (row == 3 && self->values[3] == -1)) {
        /* Row locked: grey both arrows. */
        s = ((GfxSprite **)self->base.data)[left];
        for (i = 0; i < 3; i++) {
            s->tint[i] = 0.5f;
        }
        s->alpha = 1.0f;
        s = ((GfxSprite **)self->base.data)[right];
        for (i = 0; i < 3; i++) {
            s->tint[i] = 0.5f;
        }
        s->alpha = 1.0f;
        return;
    }
    if (self->values[row] == 0) {
        s = ((GfxSprite **)self->base.data)[left];
        for (i = 0; i < 3; i++) {
            s->tint[i] = 0.5f;
        }
        s->alpha = 1.0f;
    }
    /* The count is loaded sign-extended (lb). */
    if (self->values[row] == (s8)self->valueCounts[row] - 1) {
        s = ((GfxSprite **)self->base.data)[right];
        for (i = 0; i < 3; i++) {
            s->tint[i] = 0.5f;
        }
        s->alpha = 1.0f;
    }
}
