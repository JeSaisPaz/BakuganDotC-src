// bdc 0x089308ec UiBakuganSelectAnimateFocus
#include "bdc.h"

/* Per-frame focus animation of `UiBakuganSelect`: ramps `focusZoom` by 0.1
   while it is below 1 and scales the sprites of the focused grid cell (`cursor`) — its four layer
   sprites 26/46/66/94+cursor, the pair sprites 114/115+2*cursor when the cell is < 6, and sprite 25 —
   to `1 + 0.2*zoom` (capped at 1.2), bringing them to the front (posZ −200…−206), clearing the
   linear-filter flag on some, and re-offsetting the badge sprites from `spritePos` by `layerOffset`
   times the scale. Sprite 24 gets the same scale (posZ −203) only when the cursor is on the cell of
   the current Bakugan (`UiBakuganListOrder` of `currentPos`). */

void UiBakuganSelectAnimateFocus(UiBakuganSelect *self)
{
  GfxSprite *sprite;
  s8 cursor;
  float zoom;
  float scale;

  zoom = self->focusZoom;
  if (zoom < 1.0f) {
    zoom = zoom + 0.1f;
    self->focusZoom = zoom;
  }
  scale = zoom * 0.20000005f + 1.0f;
  if (!(scale <= 1.2f)) {
    scale = 1.2f;
  }

  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->cursor + 26], scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[self->cursor + 26]->posZ = -200.0f;

  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->cursor + 46], scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[self->cursor + 46]->posZ = -201.0f;

  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->cursor + 66], scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[self->cursor + 66]->flags &= ~0x20u;
  ((GfxSprite **)self->base.data)[self->cursor + 66]->posZ = -202.0f;

  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->cursor + 94], scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[self->cursor + 94]->flags &= ~0x20u;
  ((GfxSprite **)self->base.data)[self->cursor + 94]->posZ = -205.0f;
  ((GfxSprite **)self->base.data)[self->cursor + 94]->posX =
      self->spritePos[self->cursor + 26][0] + self->layerOffset[0][0] * scale;
  ((GfxSprite **)self->base.data)[self->cursor + 94]->posY =
      self->spritePos[self->cursor + 26][1] + self->layerOffset[0][1] * scale;

  if (self->cursor < 6) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->cursor * 2 + 114], scale, scale,
                             0.0f);
    ((GfxSprite **)self->base.data)[self->cursor * 2 + 114]->posZ = -206.0f;
    ((GfxSprite **)self->base.data)[self->cursor * 2 + 114]->posX =
        self->spritePos[self->cursor + 26][0] + self->layerOffset[1][0] * scale;
    ((GfxSprite **)self->base.data)[self->cursor * 2 + 114]->posY =
        self->spritePos[self->cursor + 26][1] + self->layerOffset[1][1] * scale;

    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->cursor * 2 + 115], scale, scale,
                             0.0f);
    ((GfxSprite **)self->base.data)[self->cursor * 2 + 115]->posZ = -206.0f;
    ((GfxSprite **)self->base.data)[self->cursor * 2 + 115]->posX =
        self->spritePos[self->cursor + 26][0] + self->layerOffset[2][0] * scale;
    ((GfxSprite **)self->base.data)[self->cursor * 2 + 115]->posY =
        self->spritePos[self->cursor + 26][1] + self->layerOffset[2][1] * scale;
  }

  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[25], scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[25]->posZ = -204.0f;

  cursor = self->cursor;
  if (cursor == UiBakuganListOrder(self, false, self->currentPos)) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[24], scale, scale, 0.0f);
    ((GfxSprite **)self->base.data)[24]->posZ = -203.0f;
    sprite = ((GfxSprite **)self->base.data)[24];
    sprite->flags &= ~0x20u;
  }
}
