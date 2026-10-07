// bdc 0x0896a438 UiCardEquipGreyGaugeLimits
#include "bdc.h"

/* Resets the two gauge arrow sprites of Bakugan `index` in `UiCardEquip` to white
   and greys out (tint 0.5) the left arrow when the G-power gauge is at its minimum 50 and the right
   arrow at its maximum 150. */

void UiCardEquipGreyGaugeLimits(UiCardEquip *self, u8 index)
{
    GfxSprite **sprites = (GfxSprite **)self->base.data;
    s32 base = index * 6;
    GfxSprite *left = sprites[self->groups[14][0] + base + 2];
    GfxSprite *right;
    s32 k;

    for (k = 0; k < 3; k++) {
        left->tint[k] = 1.0f;
    }
    left->alpha = 1.0f;
    right = sprites[self->groups[14][0] + base + 5];
    for (k = 0; k < 3; k++) {
        right->tint[k] = 1.0f;
    }
    right->alpha = 1.0f;
    if (self->gauge[index] == 50) {
        left = sprites[self->groups[14][0] + base + 2];
        left->alpha = 1.0f;
        for (k = 0; k < 3; k++) {
            left->tint[k] = 0.5f;
        }
    }
    if (self->gauge[index] == 150) {
        right = sprites[self->groups[14][0] + base + 5];
        right->alpha = 1.0f;
        for (k = 0; k < 3; k++) {
            right->tint[k] = 0.5f;
        }
    }
}
