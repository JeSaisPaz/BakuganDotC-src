// bdc 0x08922c98 UiHologramGalleryTweenBoard
#include "bdc.h"

/* Starts the open (`hide` 0) or close tweens of the hologram board of the hologram gallery
   screen (task 391, `UiHologramGalleryCtor`). Every board sprite group is tweened with
   `UiTweenBeginSlide` (scale 1.0, flags 9, tween slot = sprite index): opening slides in from
   -32 px, closing slides out to -32 px with `fadeOut = hide`. A group's sprites take part only
   when their entry exists: `boardIcons[i] != 0xff` (sprites 0xa2.. and 0x45..), `slotIds[i] !=
   0xff` (0x9f..), `i < slotCount` (0xaa..), `i < slotCount` and the save profile's
   `placedHolograms[i]` empty (0x62..) or set (0xa6.., 0x41..), `boardMarks[i][0] != 0xff`
   (0xae.., 0x93.., 0x9c..); sprites 0xb1/0xb2 always. On open each such sprite is first made
   visible on layer 2, given its picture (`UiHologramGallerySetBoardIcon`,
   `UiHologramGallerySetHologramCell`, `UiHologramGallerySetFieldPicture`,
   `UiHologramGallerySetHologramIcon`, `GfxSpriteSetCell`) and placed at its `boardPos`
   entry (8-pixel units, some minus an `infoOffset`), with a depth from its row. */

#define BOARD_SPRITE(n) (((UiHologramGalleryData *)self->base.data)->sprites[(n)])

void UiHologramGalleryTweenBoard(UiHologramGallery *self, u8 hide)
{
  s32 i;
  u8 k;
  GfxSprite *sprite;
  u8 variant;
  u8 attr;

  if (hide == 0) {
    /* board icons */
    for (i = 0xa2; i < 0xa6; i++) {
      k = i - 0xa2;
      if (self->boardIcons[k] != 0xff) {
        BOARD_SPRITE(i)->flags |= 1;
        BOARD_SPRITE(i)->layerMask = 2;
        UiHologramGallerySetBoardIcon(self, BOARD_SPRITE(i), self->boardIcons[k]);
        BOARD_SPRITE(i)->posX = (float)(self->boardPos[k][0] * 8);
        BOARD_SPRITE(i)->posY = (float)(self->boardPos[k][1] * 8);
        BOARD_SPRITE(i)->posZ = -2.0f - BOARD_SPRITE(i)->posY * 0.125f * 7.0f;
        UiTweenBeginSlide(1.0f, -32.0f, 0.0f, hide, BOARD_SPRITE(i), &self->tweens[i], 9);
      }
    }
    /* holograms on the board icons */
    for (i = 0x45; i < 0x49; i++) {
      k = i - 0x45;
      if (self->boardIcons[k] != 0xff) {
        BOARD_SPRITE(i)->flags |= 1;
        BOARD_SPRITE(i)->layerMask = 2;
        UiHologramGallerySetHologramCell(self, BOARD_SPRITE(i), self->boardIcons[k], 2);
        BOARD_SPRITE(i)->posX = (float)(self->boardPos[k][0] * 8) - self->infoOffset[0][0];
        BOARD_SPRITE(i)->posY = (float)(self->boardPos[k][1] * 8) - self->infoOffset[0][1];
        BOARD_SPRITE(i)->posZ = (float)(-3 - self->boardPos[k][1] * 8 / 8 * 7);
        UiTweenBeginSlide(1.0f, -32.0f, 0.0f, hide, BOARD_SPRITE(i), &self->tweens[i], 9);
      }
    }
    /* field pictures */
    for (i = 0x9f; i < 0xa2; i++) {
      k = i - 0x9f;
      if (self->slotIds[k] != 0xff) {
        BOARD_SPRITE(i)->flags |= 1;
        BOARD_SPRITE(i)->layerMask = 2;
        UiHologramGallerySetFieldPicture(self, BOARD_SPRITE(i), self->fieldId, self->slotIds[k]);
        BOARD_SPRITE(i)->posX = (float)(self->boardPos[4 + k][0] * 8);
        BOARD_SPRITE(i)->posY = (float)(self->boardPos[4 + k][1] * 8);
        BOARD_SPRITE(i)->posZ = -2.0f - BOARD_SPRITE(i)->posY * 0.125f * 7.0f;
        UiTweenBeginSlide(1.0f, -32.0f, 0.0f, hide, BOARD_SPRITE(i), &self->tweens[i], 9);
      }
    }
    /* hologram slot frames */
    for (i = 0xaa; i < 0xae; i++) {
      k = i - 0xaa;
      if (k < self->slotCount) {
        BOARD_SPRITE(i)->flags |= 1;
        BOARD_SPRITE(i)->layerMask = 2;
        BOARD_SPRITE(i)->posX = (float)(self->boardPos[7 + k][0] * 8);
        BOARD_SPRITE(i)->posY = (float)(self->boardPos[7 + k][1] * 8 + 16);
        BOARD_SPRITE(i)->posZ = (float)(self->boardPos[7 + k][1] * 8 / 8 * -7);
        UiTweenBeginSlide(1.0f, -32.0f, 0.0f, hide, BOARD_SPRITE(i), &self->tweens[i], 9);
      }
    }
    /* empty hologram slots */
    for (i = 0x62; i < 0x66; i++) {
      k = i - 0x62;
      if (k < self->slotCount && SaveGetProfile()->data->placedHolograms[k] == 0) {
        BOARD_SPRITE(i)->flags |= 1;
        BOARD_SPRITE(i)->layerMask = 2;
        GfxSpriteSetCell(BOARD_SPRITE(i), (float)((k + 1) / 5), (float)((k + 1) % 5));
        BOARD_SPRITE(i)->posX = (float)(self->boardPos[7 + k][0] * 8);
        BOARD_SPRITE(i)->posY = (float)(self->boardPos[7 + k][1] * 8 + 16) - self->infoOffset[4][1];
        BOARD_SPRITE(i)->posZ = (float)(-1 - self->boardPos[7 + k][1] * 8 / 8 * 7);
        UiTweenBeginSlide(1.0f, -32.0f, 0.0f, hide, BOARD_SPRITE(i), &self->tweens[i], 9);
      }
    }
    /* placed hologram icons */
    for (i = 0xa6; i < 0xaa; i++) {
      k = i - 0xa6;
      if (k < self->slotCount && SaveGetProfile()->data->placedHolograms[k] != 0) {
        BOARD_SPRITE(i)->flags |= 1;
        BOARD_SPRITE(i)->layerMask = 2;
        sprite = BOARD_SPRITE(i);
        variant = (SaveGetProfile()->data->placedHolograms[k] - 14) % 3;
        attr = UiHologramGalleryMapAttribute(true, (SaveGetProfile()->data->placedHolograms[k] - 14) / 3);
        UiHologramGallerySetHologramIcon(self, sprite, variant, attr);
        BOARD_SPRITE(i)->posX = (float)(self->boardPos[7 + k][0] * 8);
        BOARD_SPRITE(i)->posY = (float)(self->boardPos[7 + k][1] * 8 + 16) - self->infoOffset[2][1];
        BOARD_SPRITE(i)->posZ = (float)(-2 - self->boardPos[7 + k][1] * 8 / 8 * 7);
        UiTweenBeginSlide(1.0f, -32.0f, 0.0f, hide, BOARD_SPRITE(i), &self->tweens[i], 9);
      }
    }
    /* placed hologram cells */
    for (i = 0x41; i < 0x45; i++) {
      k = i - 0x41;
      if (k < self->slotCount && SaveGetProfile()->data->placedHolograms[k] != 0) {
        BOARD_SPRITE(i)->flags |= 1;
        BOARD_SPRITE(i)->layerMask = 2;
        sprite = BOARD_SPRITE(i);
        attr = UiHologramGalleryMapAttribute(true, (SaveGetProfile()->data->placedHolograms[k] - 14) / 3);
        UiHologramGallerySetHologramCell(self, sprite, attr, 0);
        BOARD_SPRITE(i)->posX = (float)(self->boardPos[7 + k][0] * 8) - self->infoOffset[3][0];
        BOARD_SPRITE(i)->posY = (float)(self->boardPos[7 + k][1] * 8 + 16) - self->infoOffset[3][1];
        BOARD_SPRITE(i)->posZ = (float)(-3 - self->boardPos[7 + k][1] * 8 / 8 * 7);
        UiTweenBeginSlide(1.0f, -32.0f, 0.0f, hide, BOARD_SPRITE(i), &self->tweens[i], 9);
      }
    }
    /* board marks */
    for (i = 0xae; i < 0xb1; i++) {
      k = i - 0xae;
      if (self->boardMarks[k][0] != 0xff) {
        BOARD_SPRITE(i)->flags |= 1;
        BOARD_SPRITE(i)->layerMask = 2;
        BOARD_SPRITE(i)->posX = (float)(self->boardPos[12 + k][0] * 8);
        BOARD_SPRITE(i)->posY = (float)(self->boardPos[12 + k][1] * 8);
        BOARD_SPRITE(i)->posZ = -4.0f - BOARD_SPRITE(i)->posY * 0.125f * 7.0f;
        UiTweenBeginSlide(1.0f, -32.0f, 0.0f, hide, BOARD_SPRITE(i), &self->tweens[i], 9);
      }
    }
    for (i = 0x93; i < 0x96; i++) {
      k = i - 0x93;
      if (self->boardMarks[k][0] != 0xff) {
        BOARD_SPRITE(i)->flags |= 1;
        GfxSpriteSetCell(BOARD_SPRITE(i), (float)(self->boardMarks[k][1] / 3),
                         (float)(self->boardMarks[k][1] % 3));
        BOARD_SPRITE(i)->layerMask = 2;
        BOARD_SPRITE(i)->posX = (float)(self->boardPos[12 + k][0] * 8);
        BOARD_SPRITE(i)->posY = (float)(self->boardPos[12 + k][1] * 8);
        BOARD_SPRITE(i)->posZ = -5.0f - BOARD_SPRITE(i)->posY * 0.125f * 7.0f;
        UiTweenBeginSlide(1.0f, -32.0f, 0.0f, hide, BOARD_SPRITE(i), &self->tweens[i], 9);
      }
    }
    for (i = 0x9c; i < 0x9f; i++) {
      k = i - 0x9c;
      if (self->boardMarks[k][0] != 0xff) {
        BOARD_SPRITE(i)->flags |= 1;
        UiHologramGallerySetHologramCell(self, BOARD_SPRITE(i), self->boardMarks[k][1], 0);
        BOARD_SPRITE(i)->layerMask = 2;
        sprite = BOARD_SPRITE(i);
        sprite->tint[0] = 0.0f;
        sprite->tint[1] = 0.0f;
        sprite->tint[2] = 0.0f;
        sprite->alpha = 0.0f;
        BOARD_SPRITE(i)->posX = (float)(self->boardPos[12 + k][0] * 8);
        BOARD_SPRITE(i)->posY = (float)(self->boardPos[12 + k][1] * 8);
        BOARD_SPRITE(i)->posZ = -6.0f - BOARD_SPRITE(i)->posY * 0.125f * 7.0f;
        UiTweenBeginSlide(1.0f, -32.0f, 0.0f, hide, BOARD_SPRITE(i), &self->tweens[i], 9);
      }
    }
    /* sprites 0xb1/0xb2 at boardPos[11] */
    for (i = 0xb1; i < 0xb3; i++) {
      BOARD_SPRITE(i)->flags |= 1;
      BOARD_SPRITE(i)->layerMask = 2;
      if (i == 0xb1) {
        BOARD_SPRITE(i)->posX = (float)(self->boardPos[11][0] * 8);
        BOARD_SPRITE(i)->posY = (float)(self->boardPos[11][1] * 8);
        BOARD_SPRITE(i)->posZ = BOARD_SPRITE(i)->posY * 0.125f * -7.0f;
      } else {
        BOARD_SPRITE(i)->posX = (float)(self->boardPos[11][0] * 8) - self->infoOffset[1][0];
        BOARD_SPRITE(i)->posY = (float)(self->boardPos[11][1] * 8) - self->infoOffset[1][1];
        BOARD_SPRITE(i)->posZ = (float)(self->boardPos[11][1] * 8 / 8 * -7);
      }
      UiTweenBeginSlide(1.0f, -32.0f, 0.0f, hide, BOARD_SPRITE(i), &self->tweens[i], 9);
    }
  } else {
    for (i = 0xa2; i < 0xa6; i++) {
      if (self->boardIcons[(u8)(i - 0xa2)] != 0xff) {
        UiTweenBeginSlide(1.0f, 0.0f, -32.0f, hide, BOARD_SPRITE(i), &self->tweens[i], 9);
      }
    }
    for (i = 0x45; i < 0x49; i++) {
      if (self->boardIcons[(u8)(i - 0x45)] != 0xff) {
        UiTweenBeginSlide(1.0f, 0.0f, -32.0f, hide, BOARD_SPRITE(i), &self->tweens[i], 9);
      }
    }
    for (i = 0x9f; i < 0xa2; i++) {
      if (self->slotIds[(u8)(i - 0x9f)] != 0xff) {
        UiTweenBeginSlide(1.0f, 0.0f, -32.0f, hide, BOARD_SPRITE(i), &self->tweens[i], 9);
      }
    }
    for (i = 0xaa; i < 0xae; i++) {
      if ((u8)(i - 0xaa) < self->slotCount) {
        UiTweenBeginSlide(1.0f, 0.0f, -32.0f, hide, BOARD_SPRITE(i), &self->tweens[i], 9);
      }
    }
    for (i = 0x62; i < 0x66; i++) {
      k = i - 0x62;
      if (k < self->slotCount && SaveGetProfile()->data->placedHolograms[k] == 0) {
        UiTweenBeginSlide(1.0f, 0.0f, -32.0f, hide, BOARD_SPRITE(i), &self->tweens[i], 9);
      }
    }
    for (i = 0xa6; i < 0xaa; i++) {
      k = i - 0xa6;
      if (k < self->slotCount && SaveGetProfile()->data->placedHolograms[k] != 0) {
        UiTweenBeginSlide(1.0f, 0.0f, -32.0f, hide, BOARD_SPRITE(i), &self->tweens[i], 9);
      }
    }
    for (i = 0x41; i < 0x45; i++) {
      k = i - 0x41;
      if (k < self->slotCount && SaveGetProfile()->data->placedHolograms[k] != 0) {
        UiTweenBeginSlide(1.0f, 0.0f, -32.0f, hide, BOARD_SPRITE(i), &self->tweens[i], 9);
      }
    }
    for (i = 0xae; i < 0xb1; i++) {
      if (self->boardMarks[(u8)(i - 0xae)][0] != 0xff) {
        UiTweenBeginSlide(1.0f, 0.0f, -32.0f, hide, BOARD_SPRITE(i), &self->tweens[i], 9);
      }
    }
    for (i = 0x93; i < 0x96; i++) {
      if (self->boardMarks[(u8)(i - 0x93)][0] != 0xff) {
        UiTweenBeginSlide(1.0f, 0.0f, -32.0f, hide, BOARD_SPRITE(i), &self->tweens[i], 9);
      }
    }
    for (i = 0x9c; i < 0x9f; i++) {
      if (self->boardMarks[(u8)(i - 0x9c)][0] != 0xff) {
        UiTweenBeginSlide(1.0f, 0.0f, -32.0f, hide, BOARD_SPRITE(i), &self->tweens[i], 9);
      }
    }
    for (i = 0xb1; i < 0xb3; i++) {
      UiTweenBeginSlide(1.0f, 0.0f, -32.0f, hide, BOARD_SPRITE(i), &self->tweens[i], 9);
    }
  }
}

#undef BOARD_SPRITE
