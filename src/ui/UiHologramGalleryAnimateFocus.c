// bdc 0x08924664 UiHologramGalleryAnimateFocus
#include "bdc.h"

/* Per-frame focus animation of the hologram gallery screen: ramps `focusScale` toward 1.0
   (+0.1 per frame while below 1.0), maps it to a sprite scale of 1.0 + 0.2 * focusScale clamped
   to 1.2, and applies that scale (rotation 0) to the sprites of the focused element of the
   current `panel`, pulling each to the front with a fixed `posZ` between -100 and -105.
   Panel 0: the `menuCursor` entry (menu buttons 180/183/185 + cursor, 182) or, for cursors >= 2,
   sprites 128/130/131. Panel 1: in slot mode (`listMode == 0`) the `slot` row (sprites 103,
   108, 116, 112, 124, 90, 120, 8 + slot, with the row's child sprites re-placed at
   `slotRowOrigin.x - slotRowOffset[k].x * scaleX` and `row.posY - slotRowOffset[k].y * scaleY`)
   plus sprite 107; in list mode sprites 128/129/131. Panel 2: the `helpCursor` entry
   (134/141/150 + cursor) plus sprite 140. Other panels: nothing. Unfocused sprites are not
   touched. */

#define SPRITE(i) (((GfxSprite **)self->base.data)[i])

void UiHologramGalleryAnimateFocus(UiHologramGallery *self)
{
  float scale;
  GfxSprite *sprite;

  scale = self->focusScale;
  if (scale < 1.0f) {
    scale = scale + 0.1f;
    self->focusScale = scale;
  }
  scale = scale * 0.20000005f + 1.0f;
  if (!(scale <= 1.2f)) {
    scale = 1.2f;
  }

  if (self->panel <= 0) {
    if (self->panel < 0) {
      return;
    }
    if (self->menuCursor < 2) {
      UiSpriteSetScaleRotation(SPRITE(180 + self->menuCursor), scale, scale, 0.0f);
      SPRITE(180 + self->menuCursor)->posZ = -100.0f;
      UiSpriteSetScaleRotation(SPRITE(183 + self->menuCursor), scale, scale, 0.0f);
      SPRITE(183 + self->menuCursor)->posZ = -101.0f;
      UiSpriteSetScaleRotation(SPRITE(185 + self->menuCursor), scale, scale, 0.0f);
      SPRITE(185 + self->menuCursor)->posZ = -102.0f;
      UiSpriteSetScaleRotation(SPRITE(182), scale, scale, 0.0f);
      SPRITE(182)->posZ = -103.0f;
    }
    else {
      UiSpriteSetScaleRotation(SPRITE(128), scale, scale, 0.0f);
      SPRITE(128)->posZ = -100.0f;
      UiSpriteSetScaleRotation(SPRITE(130), scale, scale, 0.0f);
      SPRITE(130)->posZ = -101.0f;
      UiSpriteSetScaleRotation(SPRITE(131), scale, scale, 0.0f);
      SPRITE(131)->posZ = -102.0f;
    }
  }
  else if (self->panel < 2) {
    if (self->listMode[0] == 0) {
      UiSpriteSetScaleRotation(SPRITE(103 + self->slot), scale, scale, 0.0f);
      SPRITE(103 + self->slot)->posZ = -100.0f;

      UiSpriteSetScaleRotation(SPRITE(108 + self->slot), scale, scale, 0.0f);
      SPRITE(108 + self->slot)->posZ = -101.0f;
      sprite = SPRITE(108 + self->slot);
      sprite->posX = self->slotRowOrigin[0] - self->slotRowOffset[0][0] * sprite->scaleX;
      sprite = SPRITE(108 + self->slot);
      sprite->posY = SPRITE(103 + self->slot)->posY - self->slotRowOffset[0][1] * sprite->scaleY;

      UiSpriteSetScaleRotation(SPRITE(116 + self->slot), scale, scale, 0.0f);
      SPRITE(116 + self->slot)->posZ = -101.0f;
      sprite = SPRITE(116 + self->slot);
      sprite->posX = self->slotRowOrigin[0] - self->slotRowOffset[1][0] * sprite->scaleX;
      sprite = SPRITE(116 + self->slot);
      sprite->posY = SPRITE(103 + self->slot)->posY - self->slotRowOffset[1][1] * sprite->scaleY;

      UiSpriteSetScaleRotation(SPRITE(112 + self->slot), scale, scale, 0.0f);
      SPRITE(112 + self->slot)->posZ = -101.0f;
      sprite = SPRITE(112 + self->slot);
      sprite->posX = self->slotRowOrigin[0] - self->slotRowOffset[2][0] * sprite->scaleX;
      sprite = SPRITE(112 + self->slot);
      sprite->posY = SPRITE(103 + self->slot)->posY - self->slotRowOffset[2][1] * sprite->scaleY;

      UiSpriteSetScaleRotation(SPRITE(124 + self->slot), scale, scale, 0.0f);
      SPRITE(124 + self->slot)->posZ = -101.0f;
      sprite = SPRITE(124 + self->slot);
      sprite->posX = self->slotRowOrigin[0] - self->slotRowOffset[3][0] * sprite->scaleX;
      sprite = SPRITE(124 + self->slot);
      sprite->posY = SPRITE(103 + self->slot)->posY - self->slotRowOffset[3][1] * sprite->scaleY;

      UiSpriteSetScaleRotation(SPRITE(90 + self->slot), scale, scale, 0.0f);
      SPRITE(90 + self->slot)->posZ = -102.0f;
      sprite = SPRITE(90 + self->slot);
      sprite->posX = self->slotRowOrigin[0] - self->slotRowOffset[4][0] * sprite->scaleX;
      sprite = SPRITE(90 + self->slot);
      sprite->posY = SPRITE(103 + self->slot)->posY - self->slotRowOffset[4][1] * sprite->scaleY;

      UiSpriteSetScaleRotation(SPRITE(120 + self->slot), scale, scale, 0.0f);
      SPRITE(120 + self->slot)->posZ = -104.0f;
      sprite = SPRITE(120 + self->slot);
      sprite->posX = self->slotRowOrigin[0] - self->slotRowOffset[5][0] * sprite->scaleX;
      sprite = SPRITE(120 + self->slot);
      sprite->posY = SPRITE(103 + self->slot)->posY - self->slotRowOffset[5][1] * sprite->scaleY;

      UiSpriteSetScaleRotation(SPRITE(8 + self->slot), scale, scale, 0.0f);
      SPRITE(8 + self->slot)->posZ = -105.0f;
      sprite = SPRITE(8 + self->slot);
      sprite->posX = self->slotRowOrigin[0] - self->slotRowOffset[6][0] * sprite->scaleX;
      sprite = SPRITE(8 + self->slot);
      sprite->posY = SPRITE(103 + self->slot)->posY - self->slotRowOffset[6][1] * sprite->scaleY;

      UiSpriteSetScaleRotation(SPRITE(107), scale, scale, 0.0f);
      SPRITE(107)->posZ = -103.0f;
    }
    else {
      UiSpriteSetScaleRotation(SPRITE(128), scale, scale, 0.0f);
      SPRITE(128)->posZ = -100.0f;
      UiSpriteSetScaleRotation(SPRITE(129), scale, scale, 0.0f);
      SPRITE(129)->posZ = -101.0f;
      UiSpriteSetScaleRotation(SPRITE(131), scale, scale, 0.0f);
      SPRITE(131)->posZ = -102.0f;
    }
  }
  else if (self->panel < 3) {
    UiSpriteSetScaleRotation(SPRITE(134 + self->helpCursor), scale, scale, 0.0f);
    SPRITE(134 + self->helpCursor)->posZ = -100.0f;
    UiSpriteSetScaleRotation(SPRITE(141 + self->helpCursor), scale, scale, 0.0f);
    SPRITE(141 + self->helpCursor)->posZ = -101.0f;
    UiSpriteSetScaleRotation(SPRITE(150 + self->helpCursor), scale, scale, 0.0f);
    SPRITE(150 + self->helpCursor)->posZ = -102.0f;
    UiSpriteSetScaleRotation(SPRITE(140), scale, scale, 0.0f);
    SPRITE(140)->posZ = -103.0f;
  }
}
