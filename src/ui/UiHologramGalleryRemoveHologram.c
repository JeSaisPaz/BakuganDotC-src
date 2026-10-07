// bdc 0x089264f0 UiHologramGalleryRemoveHologram
#include "bdc.h"

/* Removes the hologram from board slot `slot` of the hologram gallery screen (task 391,
   `UiHologramGalleryCtor`; step 0x11 of `UiHologramGalleryMainPhase`), stepped by
   `confirm[0]`:
   0: places sprite 0x6c + slot on the slot row (shown, alpha 1) and hides sprites 0x74/0x70/0x78/8
      + slot;
   1: clears the profile's `placedHolograms[slot]`, starts the fade-out of sprites 0xa6/0x41 + slot
      and the grow-in of sprite 0x62 + slot (cell slot + 1, at board position 7 + slot);
   2: steps those three tweens and the refund count (`UiHologramGalleryCountRefund`); when all
      four report done the step becomes 3.
   Returns 1 once the step is 3 or more, else 0. */

s32 UiHologramGalleryRemoveHologram(UiHologramGallery *self)
{
    GfxSprite **sprites;
    GfxSprite *sprite;
    SaveProfile *profile;
    s32 idx;
    s32 rowY;
    s32 n;
    u8 step;
    u8 done;

    sprites = (GfxSprite **)self->base.data;
    step = self->confirm[0];
    if (step == 0) {
        idx = self->slot + 0x6c;
        sprites[idx]->flags |= 1;
        sprites[idx]->alpha = 1.0f;
        rowY = (s32)(self->slotRowOrigin[1] - (float)((s32)self->slotCount * 13 - 13));
        sprites[idx]->posX = self->slotRowOrigin[0] - self->slotRowOffset[0][0];
        sprites[idx]->posY = (float)(rowY + self->slot * 26) - self->slotRowOffset[0][1];
        sprites[self->slot + 0x74]->flags &= ~1u;
        sprites[self->slot + 0x70]->flags &= ~1u;
        sprites[self->slot + 0x78]->flags &= ~1u;
        sprites[self->slot + 8]->flags &= ~1u;
        self->confirm[0] = self->confirm[0] + 1;
    } else if (step < 2) {
        profile = SaveGetProfile();
        profile->data->placedHolograms[(u8)self->slot] = 0;
        UiTweenBegin(1.0f, 1, sprites[self->slot + 0xa6], &self->tweens[self->slot + 0xa6], 3);
        UiTweenBegin(1.0f, 1, sprites[self->slot + 0x41], &self->tweens[self->slot + 0x41], 3);
        idx = self->slot + 0x62;
        sprites[idx]->flags |= 1;
        sprites[idx]->alpha = 0.0f;
        sprites[idx]->layerMask = 2;
        UiSpriteSetScaleRotation(sprites[idx], 0.0f, 0.0f, 0.0f);
        n = self->slot + 1;
        GfxSpriteSetCell(sprites[idx], (float)(n / 5), (float)(n % 5));
        sprites[idx]->posX = (float)(self->boardPos[self->slot + 7][0] * 8);
        sprite = sprites[idx];
        sprite->posY = (float)(self->boardPos[self->slot + 7][1] * 8 + 16)
                     - self->infoOffset[4][1] * sprite->scaleX;
        sprites[idx]->posZ = (float)(-1 - ((self->boardPos[self->slot + 7][1] * 8) / 8) * 7);
        UiTweenBegin(0.0f, 0, sprites[idx], &self->tweens[idx], 3);
        self->confirm[0] = self->confirm[0] + 1;
    } else {
        if (step >= 3) {
            return 1;
        }
        idx = self->slot + 0xa6;
        done = UiTweenUpdate(1.0f, 0.0f, 30.0f, 1, sprites[idx], &self->tweens[idx], 3);
        sprite = sprites[idx];
        sprite->posY = (float)(self->boardPos[self->slot + 7][1] * 8 + 16)
                     - self->infoOffset[2][1] * sprite->scaleX;

        idx = self->slot + 0x41;
        n = UiTweenUpdate(1.0f, 0.0f, 30.0f, 1, sprites[idx], &self->tweens[idx], 3);
        sprite = sprites[idx];
        sprite->posX = (float)(self->boardPos[self->slot + 7][0] * 8)
                     - self->infoOffset[3][0] * sprite->scaleX;
        done = (u8)(done + n);
        sprite = sprites[idx];
        sprite->posY = (float)(self->boardPos[self->slot + 7][1] * 8 + 16)
                     - self->infoOffset[3][1] * sprite->scaleX;

        idx = self->slot + 0x62;
        n = UiTweenUpdate(0.0f, 1.0f, 30.0f, 0, sprites[idx], &self->tweens[idx], 3);
        sprite = sprites[idx];
        sprite->posY = (float)(self->boardPos[self->slot + 7][1] * 8 + 16)
                     - self->infoOffset[4][1] * sprite->scaleX;
        done = (u8)(done + n);

        if ((u8)(done + UiHologramGalleryCountRefund(self, 1)) == 4) {
            self->confirm[0] = 3;
        }
    }
    return 0;
}
