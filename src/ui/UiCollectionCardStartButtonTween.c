// bdc 0x0898328c UiCollectionCardStartButtonTween
#include "bdc.h"

/* Starts the zoom tween (`UiTweenBegin`, scale 1.5, flags 3, tween record = sprite index) of the
   card cells of the current page of the card collection screen (task 313, `maybe_UiScreen313Ctor`)
   (`out` = 0 in, 1 out). Sprites: 0..3 cell frames, 23..26 card labels, 5..8 and 9..12 per-slot
   markers. On the way in it first resets them: frames made visible, set unselected
   (`UiCollectionCardSetCellFrame`) with `addColor` cleared; labels made visible and set to the
   slot's card (`UiCollectionCardSetCardLabel`); sprites 5..8 visible only when the slot is empty
   (`slots` == 0xff); sprites 9..12 visible only when the slot's `isNew` is set; every one gets its
   saved depth `spriteZ[i]` as `posZ`. On the way out only the tweens are started. */

void UiCollectionCardStartButtonTween(UiCollectionCard *self, u8 out)
{
    GfxSprite *sprite;
    int i;

    if (out == 0) {
        for (i = 0; i < 4; i++) {
            ((GfxSprite **)self->base.data)[i]->flags |= 1;
            UiCollectionCardSetCellFrame(self, ((GfxSprite **)self->base.data)[i], 0);
            ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
            sprite = ((GfxSprite **)self->base.data)[i];
            sprite->addColor[0] = 0.0f;
            sprite->addColor[1] = 0.0f;
            sprite->addColor[2] = 0.0f;
            sprite->addColor[3] = 0.0f;
            UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
        }
        for (i = 23; i < 27; i++) {
            ((GfxSprite **)self->base.data)[i]->flags |= 1;
            UiCollectionCardSetCardLabel(self, ((GfxSprite **)self->base.data)[i],
                                         self->slots[(i - 23) + self->page * 4]);
            ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
            UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
        }
        for (i = 5; i < 9; i++) {
            sprite = ((GfxSprite **)self->base.data)[i];
            if (self->slots[(i - 5) + self->page * 4] == 0xff) {
                sprite->flags |= 1;
            } else {
                sprite->flags &= ~1u;
            }
            ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
            UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
        }
        for (i = 9; i < 13; i++) {
            sprite = ((GfxSprite **)self->base.data)[i];
            if (self->isNew[(i - 9) + self->page * 4] != 0) {
                sprite->flags |= 1;
            } else {
                sprite->flags &= ~1u;
            }
            ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
            UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
        }
    } else {
        for (i = 0; i < 4; i++) {
            UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
        }
        for (i = 23; i < 27; i++) {
            UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
        }
        for (i = 5; i < 9; i++) {
            UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
        }
        for (i = 9; i < 13; i++) {
            UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
        }
    }
}
