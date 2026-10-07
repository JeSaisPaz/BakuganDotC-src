// bdc 0x089278b0 UiHologramGalleryPlaceHologram
#include "bdc.h"

/* Places the selected hologram into board slot `slot` of the hologram gallery screen (task 391,
   `UiHologramGalleryCtor`; called from `UiHologramGalleryMainPhase`), driven by `confirm[0]`:
   step 0 writes save-profile byte `placedHolograms[slot] = 0xe + attr*3 + helpPage` (attr from
   `UiHologramGalleryMapAttribute` of `helpOrder[helpCursor]`), sets up the slot's hologram
   icon (sprite 0xa6 + slot, `UiHologramGallerySetHologramIcon`) and attribute cell (sprite
   0x41 + slot, `UiHologramGallerySetHologramCell`) at `boardPos[7 + slot]` (8-pixel units),
   starts their pop-in tweens and the fade-out of sprite 0x62 + slot; step 1 runs the three
   30-frame tweens plus `UiHologramGalleryCountRefund` until all four report done; step 2 sets
   profile `viewSeenMask` bit 0 and recomputes the menu locks when the tutorial lock is active,
   and returns 1. Every other call returns 0. */

s32 UiHologramGalleryPlaceHologram(UiHologramGallery *self)
{
  SaveProfile *profile;
  GfxSprite *sprite;
  int attr;
  int i;
  u8 slot;
  u8 variant;
  u8 done;

  if (self->confirm[0] == 0) {
    profile = SaveGetProfile();
    slot = (u8)self->slot;
    attr = UiHologramGalleryMapAttribute(false, self->helpOrder[self->helpCursor]);
    profile->data->placedHolograms[slot] = (u8)(attr * 3 + self->helpPage + 0xe);

    /* hologram icon sprite */
    i = self->slot + 0xa6;
    ((GfxSprite **)self->base.data)[i]->flags |= 1;
    ((GfxSprite **)self->base.data)[i]->alpha = 0.0f;
    ((GfxSprite **)self->base.data)[i]->layerMask = 2;
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 0.0f, 0.0f, 0.0f);
    sprite = ((GfxSprite **)self->base.data)[i];
    profile = SaveGetProfile();
    variant = (u8)((profile->data->placedHolograms[(u8)self->slot] - 0xe) % 3);
    profile = SaveGetProfile();
    attr = UiHologramGalleryMapAttribute(
        true, (u8)((profile->data->placedHolograms[(u8)self->slot] - 0xe) / 3));
    UiHologramGallerySetHologramIcon(self, sprite, variant, (u8)attr);
    ((GfxSprite **)self->base.data)[i]->posX = (float)(self->boardPos[self->slot + 7][0] * 8);
    sprite = ((GfxSprite **)self->base.data)[i];
    sprite->posY = (float)(self->boardPos[self->slot + 7][1] * 8 + 0x10) -
                   self->infoOffset[2][1] * sprite->scaleX;
    ((GfxSprite **)self->base.data)[i]->posZ =
        (float)(-2 - (self->boardPos[self->slot + 7][1] * 8) / 8 * 7);
    UiTweenBegin(0.0f, 0, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);

    /* attribute cell sprite */
    i = self->slot + 0x41;
    ((GfxSprite **)self->base.data)[i]->flags |= 1;
    ((GfxSprite **)self->base.data)[i]->layerMask = 2;
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 0.0f, 0.0f, 0.0f);
    sprite = ((GfxSprite **)self->base.data)[i];
    profile = SaveGetProfile();
    attr = UiHologramGalleryMapAttribute(
        true, (u8)((profile->data->placedHolograms[(u8)self->slot] - 0xe) / 3));
    UiHologramGallerySetHologramCell(self, sprite, (u8)attr, 0);
    sprite = ((GfxSprite **)self->base.data)[i];
    sprite->posX = (float)(self->boardPos[self->slot + 7][0] * 8) -
                   self->infoOffset[3][0] * sprite->scaleX;
    sprite = ((GfxSprite **)self->base.data)[i];
    sprite->posY = (float)(self->boardPos[self->slot + 7][1] * 8 + 0x10) -
                   self->infoOffset[3][1] * sprite->scaleX;
    ((GfxSprite **)self->base.data)[i]->posZ =
        (float)(-3 - (self->boardPos[self->slot + 7][1] * 8) / 8 * 7);
    UiTweenBegin(0.0f, 0, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);

    /* sprite 0x62 + slot fades out */
    i = self->slot + 0x62;
    UiTweenBegin(1.0f, 1, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    self->confirm[0]++;
  } else if (self->confirm[0] < 2) {
    i = self->slot + 0xa6;
    done = UiTweenUpdate(0.0f, 1.0f, 30.0f, 0, ((GfxSprite **)self->base.data)[i],
                         &self->tweens[i], 3);
    sprite = ((GfxSprite **)self->base.data)[i];
    sprite->posY = (float)(self->boardPos[self->slot + 7][1] * 8 + 0x10) -
                   self->infoOffset[2][1] * sprite->scaleX;

    i = self->slot + 0x41;
    done += UiTweenUpdate(0.0f, 1.0f, 30.0f, 0, ((GfxSprite **)self->base.data)[i],
                          &self->tweens[i], 3);
    sprite = ((GfxSprite **)self->base.data)[i];
    sprite->posX = (float)(self->boardPos[self->slot + 7][0] * 8) -
                   self->infoOffset[3][0] * sprite->scaleX;
    sprite = ((GfxSprite **)self->base.data)[i];
    sprite->posY = (float)(self->boardPos[self->slot + 7][1] * 8 + 0x10) -
                   self->infoOffset[3][1] * sprite->scaleX;

    i = self->slot + 0x62;
    done += UiTweenUpdate(1.0f, 0.0f, 30.0f, 1, ((GfxSprite **)self->base.data)[i],
                          &self->tweens[i], 3);
    sprite = ((GfxSprite **)self->base.data)[i];
    sprite->posY = (float)(self->boardPos[self->slot + 7][1] * 8 + 0x10) -
                   self->infoOffset[4][1] * sprite->scaleX;

    done += UiHologramGalleryCountRefund(self, 0);
    if (done == 4) {
      self->confirm[0] = 2;
    }
  } else {
    if (UiHologramGalleryIsTutorialLocked() == 1) {
      SaveGetProfile()->data->viewSeenMask |= 1;
      UiHologramGalleryInitMenuLocks(self);
    }
    return 1;
  }
  return 0;
}
