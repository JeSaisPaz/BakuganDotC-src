// bdc 0x08971e24 UiOptionShowHelp
#include "bdc.h"

/* Shows (`show` != 0) or hides the help box of the battle-options screen (task 304,
   `maybe_UiScreen304Ctor`). Show: makes sprites 0x35..0x38 visible at alpha 1 (0x35 gets button
   icon 2, 0x36 icon 1), centres sprite 0x19 at scale 1 and shows it; when profile flag 0 is set and
   the local player index is 1, sprites 0x35..0x38 are hidden again and help message 5 is printed
   6 units above sprite 0x19 (`UiOptionPrintHelp`). Hide: hides sprites 0x35..0x38 and 0x19 and
   clears the help printer. */

void UiOptionShowHelp(UiOption *self, bool show)
{
    GfxSprite *sprite;
    s32 i;

    if (show) {
        for (i = 0x35; i < 0x39; i++) {
            sprite = ((GfxSprite **)self->base.data)[i];
            if (i < 0x36) {
                if (i >= 0x35) {
                    UiSetButtonIcon(sprite, 2);
                    sprite = ((GfxSprite **)self->base.data)[i];
                }
            } else if (i < 0x37) {
                UiSetButtonIcon(sprite, 1);
                sprite = ((GfxSprite **)self->base.data)[i];
            }
            sprite->flags |= 1;
            ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
            if (SaveGetProfileFlag0() != 0 && NetGetLocalPlayerIndex() == 1) {
                ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
            }
        }
        for (i = 0x19; i < 0x1a; i++) {
            ((GfxSprite **)self->base.data)[i]->flags |= 1;
            ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
            GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[i]);
            ((GfxSprite **)self->base.data)[i]->flags |= 0x20;
            UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
            if (SaveGetProfileFlag0() != 0 && NetGetLocalPlayerIndex() == 1) {
                sprite = ((GfxSprite **)self->base.data)[i];
                UiOptionPrintHelp(sprite->posX, sprite->posY - 6.0f, self, 5);
            }
        }
    } else {
        for (i = 0x35; i < 0x39; i++) {
            ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
        }
        for (i = 0x19; i < 0x1a; i++) {
            ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
        }
        UiOptionClearHelp(self);
    }
}
