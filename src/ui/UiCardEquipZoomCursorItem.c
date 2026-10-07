// bdc 0x0896dbdc UiCardEquipZoomCursorItem
#include "bdc.h"

/* Scales up the item under the cursor of `UiCardEquip` and sets its depth.
   Row 0: the tab highlight (group 3) at scale 1.0, depth -502. Row 1: `tabScale` grows by 0.2
   per call while below 1.0, the scale is `tabScale * 0.1 + 1.0` capped at 1.1; the card icon
   (group 10), mark (group 8), marker (group 9), groups 17 and 16 of the selected Bakugan's slot
   and the group-18 sprite get that scale, depths -500..-505, and groups 8/17/16 lose flag 0x20.
   Rows >= 2: only the scale is computed, nothing is drawn. */

void UiCardEquipZoomCursorItem(UiCardEquip *self, u8 row)
{
  float scale;

  if (row == 0) {
    scale = 1.0f;
  } else {
    scale = self->tabScale;
    if (scale < 1.0f) {
      scale = scale + 0.2f;
      self->tabScale = scale;
    }
    scale = scale * 0.100000024f + 1.0f;
    if (!(scale <= 1.1f)) {
      scale = 1.1f;
    }
  }

  if (row == 0) {
    UiSpriteSetScaleRotation(
        ((GfxSprite **)self->base.data)[self->groups[3][0] + self->rowCursor[row]],
        scale, scale, 0.0f);
    ((GfxSprite **)self->base.data)[self->groups[3][0] + self->rowCursor[row]]->posZ = -502.0f;
  } else if (row < 2) {
    UiSpriteSetScaleRotation(
        ((GfxSprite **)self->base.data)[self->groups[10][0] + self->selBakugan * 4 +
                                        self->rowCursor[row]],
        scale, scale, 0.0f);
    ((GfxSprite **)self->base.data)[self->groups[10][0] + self->selBakugan * 4 +
                                    self->rowCursor[row]]->posZ = -500.0f;

    UiSpriteSetScaleRotation(
        ((GfxSprite **)self->base.data)[self->groups[8][0] + self->selBakugan * 4 +
                                        self->rowCursor[row]],
        scale, scale, 0.0f);
    ((GfxSprite **)self->base.data)[self->groups[8][0] + self->selBakugan * 4 +
                                    self->rowCursor[row]]->flags &= ~0x20u;
    ((GfxSprite **)self->base.data)[self->groups[8][0] + self->selBakugan * 4 +
                                    self->rowCursor[row]]->posZ = -501.0f;

    UiSpriteSetScaleRotation(
        ((GfxSprite **)self->base.data)[self->groups[9][0] + self->selBakugan * 4 +
                                        self->rowCursor[row]],
        scale, scale, 0.0f);
    ((GfxSprite **)self->base.data)[self->groups[9][0] + self->selBakugan * 4 +
                                    self->rowCursor[row]]->posZ = -502.0f;

    UiSpriteSetScaleRotation(
        ((GfxSprite **)self->base.data)[self->groups[17][0] + self->selBakugan * 4 +
                                        self->rowCursor[row]],
        scale, scale, 0.0f);
    ((GfxSprite **)self->base.data)[self->groups[17][0] + self->selBakugan * 4 +
                                    self->rowCursor[row]]->flags &= ~0x20u;
    ((GfxSprite **)self->base.data)[self->groups[17][0] + self->selBakugan * 4 +
                                    self->rowCursor[row]]->posZ = -503.0f;

    UiSpriteSetScaleRotation(
        ((GfxSprite **)self->base.data)[self->groups[16][0] + self->selBakugan * 4 +
                                        self->rowCursor[row]],
        scale, scale, 0.0f);
    ((GfxSprite **)self->base.data)[self->groups[16][0] + self->selBakugan * 4 +
                                    self->rowCursor[row]]->flags &= ~0x20u;
    ((GfxSprite **)self->base.data)[self->groups[16][0] + self->selBakugan * 4 +
                                    self->rowCursor[row]]->posZ = -504.0f;

    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->groups[18][0]],
                             scale, scale, 0.0f);
    ((GfxSprite **)self->base.data)[self->groups[18][0]]->posZ = -505.0f;
  }
}
