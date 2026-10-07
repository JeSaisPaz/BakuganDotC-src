// bdc 0x08935fdc UiGauntletSetupTweenCardSlots
#include "bdc.h"

/* Starts the horizontal slide tweens of the card-slot sprite groups of the gauntlet setup screen.
   Seven groups of four sprites (0xe.., 0x2a.., 0x22.., 0x32.., 0x18.., 0xa.., 0x14..) each slide
   relative to the first sprite of their group (the group origin).
   hide == 0 (open): each sprite is first shown or hidden (`flags` bit 0, `layerMask` 2 when shown),
   moved onto the origin's X and then tweened by `spritePos[i][0] - origin X` towards its recorded
   position. Visibility: group 0xe always shown; 0x2a shown when the slot holds a card (face set by
   UiGauntletSetupSetCardTexture); 0x22 when `slotNew`; 0x32, 0x18 (icon 2 via UiSetButtonIcon) and
   0xa when `slotMark` bit 0; 0x14 when the slot is empty (`slotCard` == 0xff).
   hide != 0 (close): tweens every sprite by `origin X - spritePos[i][0]` with fade-out, without
   touching visibility. All tweens use start scale 1.0, slide from 0.0 and flags 7. */

void UiGauntletSetupTweenCardSlots(UiGauntletSetup *self, u8 hide)
{
    int i;

    if (hide == 0) {
        for (i = 0xe; i < 0x12; i++) {
            ((GfxSprite **)self->base.data)[i]->flags |= 1;
            ((GfxSprite **)self->base.data)[i]->layerMask = 2;
            ((GfxSprite **)self->base.data)[i]->posX = ((GfxSprite **)self->base.data)[0xe]->posX;
            UiTweenBeginSlide(1.0f, 0.0f,
                              self->spritePos[i][0] - ((GfxSprite **)self->base.data)[0xe]->posX, hide,
                              ((GfxSprite **)self->base.data)[i], &self->tweens[i], 7);
        }
        for (i = 0x2a; i < 0x2e; i++) {
            if (self->slotCard[i - 0x2a] != 0xff) {
                ((GfxSprite **)self->base.data)[i]->flags |= 1;
                ((GfxSprite **)self->base.data)[i]->layerMask = 2;
                UiGauntletSetupSetCardTexture(((GfxSprite **)self->base.data)[i], self->slotCard[i - 0x2a]);
            } else {
                ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
            }
            ((GfxSprite **)self->base.data)[i]->posX = ((GfxSprite **)self->base.data)[0x2a]->posX;
            UiTweenBeginSlide(1.0f, 0.0f,
                              self->spritePos[i][0] - ((GfxSprite **)self->base.data)[0x2a]->posX, hide,
                              ((GfxSprite **)self->base.data)[i], &self->tweens[i], 7);
        }
        for (i = 0x22; i < 0x26; i++) {
            if (self->slotNew[i - 0x22] != 0) {
                ((GfxSprite **)self->base.data)[i]->flags |= 1;
                ((GfxSprite **)self->base.data)[i]->layerMask = 2;
            } else {
                ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
            }
            ((GfxSprite **)self->base.data)[i]->posX = ((GfxSprite **)self->base.data)[0x22]->posX;
            UiTweenBeginSlide(1.0f, 0.0f,
                              self->spritePos[i][0] - ((GfxSprite **)self->base.data)[0x22]->posX, hide,
                              ((GfxSprite **)self->base.data)[i], &self->tweens[i], 7);
        }
        for (i = 0x32; i < 0x36; i++) {
            if (self->slotMark[i - 0x32] & 1) {
                ((GfxSprite **)self->base.data)[i]->flags |= 1;
                ((GfxSprite **)self->base.data)[i]->layerMask = 2;
            } else {
                ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
            }
            ((GfxSprite **)self->base.data)[i]->posX = ((GfxSprite **)self->base.data)[0x32]->posX;
            UiTweenBeginSlide(1.0f, 0.0f,
                              self->spritePos[i][0] - ((GfxSprite **)self->base.data)[0x32]->posX, hide,
                              ((GfxSprite **)self->base.data)[i], &self->tweens[i], 7);
        }
        for (i = 0x18; i < 0x1c; i++) {
            if (self->slotMark[i - 0x18] & 1) {
                ((GfxSprite **)self->base.data)[i]->flags |= 1;
                ((GfxSprite **)self->base.data)[i]->layerMask = 2;
                UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 2);
            } else {
                ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
            }
            ((GfxSprite **)self->base.data)[i]->posX = ((GfxSprite **)self->base.data)[0x18]->posX;
            UiTweenBeginSlide(1.0f, 0.0f,
                              self->spritePos[i][0] - ((GfxSprite **)self->base.data)[0x18]->posX, hide,
                              ((GfxSprite **)self->base.data)[i], &self->tweens[i], 7);
        }
        for (i = 0xa; i < 0xe; i++) {
            if (self->slotMark[i - 0xa] & 1) {
                ((GfxSprite **)self->base.data)[i]->flags |= 1;
                ((GfxSprite **)self->base.data)[i]->layerMask = 2;
            } else {
                ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
            }
            ((GfxSprite **)self->base.data)[i]->posX = ((GfxSprite **)self->base.data)[0xa]->posX;
            UiTweenBeginSlide(1.0f, 0.0f,
                              self->spritePos[i][0] - ((GfxSprite **)self->base.data)[0xa]->posX, hide,
                              ((GfxSprite **)self->base.data)[i], &self->tweens[i], 7);
        }
        for (i = 0x14; i < 0x18; i++) {
            if (self->slotCard[i - 0x14] == 0xff) {
                ((GfxSprite **)self->base.data)[i]->flags |= 1;
                ((GfxSprite **)self->base.data)[i]->layerMask = 2;
            } else {
                ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
            }
            ((GfxSprite **)self->base.data)[i]->posX = ((GfxSprite **)self->base.data)[0x14]->posX;
            UiTweenBeginSlide(1.0f, 0.0f,
                              self->spritePos[i][0] - ((GfxSprite **)self->base.data)[0x14]->posX, hide,
                              ((GfxSprite **)self->base.data)[i], &self->tweens[i], 7);
        }
    } else {
        for (i = 0xe; i < 0x12; i++) {
            UiTweenBeginSlide(1.0f, 0.0f,
                              ((GfxSprite **)self->base.data)[0xe]->posX - self->spritePos[i][0], hide,
                              ((GfxSprite **)self->base.data)[i], &self->tweens[i], 7);
        }
        for (i = 0x2a; i < 0x2e; i++) {
            UiTweenBeginSlide(1.0f, 0.0f,
                              ((GfxSprite **)self->base.data)[0x2a]->posX - self->spritePos[i][0], hide,
                              ((GfxSprite **)self->base.data)[i], &self->tweens[i], 7);
        }
        for (i = 0x22; i < 0x26; i++) {
            UiTweenBeginSlide(1.0f, 0.0f,
                              ((GfxSprite **)self->base.data)[0x22]->posX - self->spritePos[i][0], hide,
                              ((GfxSprite **)self->base.data)[i], &self->tweens[i], 7);
        }
        for (i = 0x32; i < 0x36; i++) {
            UiTweenBeginSlide(1.0f, 0.0f,
                              ((GfxSprite **)self->base.data)[0x32]->posX - self->spritePos[i][0], hide,
                              ((GfxSprite **)self->base.data)[i], &self->tweens[i], 7);
        }
        for (i = 0x18; i < 0x1c; i++) {
            UiTweenBeginSlide(1.0f, 0.0f,
                              ((GfxSprite **)self->base.data)[0x18]->posX - self->spritePos[i][0], hide,
                              ((GfxSprite **)self->base.data)[i], &self->tweens[i], 7);
        }
        for (i = 0xa; i < 0xe; i++) {
            UiTweenBeginSlide(1.0f, 0.0f,
                              ((GfxSprite **)self->base.data)[0xa]->posX - self->spritePos[i][0], hide,
                              ((GfxSprite **)self->base.data)[i], &self->tweens[i], 7);
        }
        for (i = 0x14; i < 0x18; i++) {
            UiTweenBeginSlide(1.0f, 0.0f,
                              ((GfxSprite **)self->base.data)[0x14]->posX - self->spritePos[i][0], hide,
                              ((GfxSprite **)self->base.data)[i], &self->tweens[i], 7);
        }
    }
}
