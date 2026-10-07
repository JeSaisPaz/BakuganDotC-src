// bdc 0x08948af4 UiBattleRecordAnimateMenu
#include "bdc.h"

/* Open (`out` = 0) / close (`out` = 1) animation of the category menu of the battle-record screen
   (task 3005, `UiBattleRecordCtor`; per-Bakugan win/loss statistics from the save profile, layout
   package `"data/2d/%s/record.lzs"`); one step per call, also stepping the title plate
   (`UiTitlePlateStep`). Open: frames 0..5 fade in sprites 0, 11, 12, 13, 16, 17 by 1/6 (all set to
   sprite 13's new alpha); frames 6..11 fade sprites 1..10 in by 1/6 and shrink them from (1.2, 1.1)
   to 1 via `UiBattleRecordScaleStep`; afterwards the cursor sprites 18/5/6 are put on entry 1 at
   1.15x, sprite 5 is made visible, and 1 is returned once the title plate is done. Close: hides
   sprite 18, grows sprites 1..10 towards (1.2, 1.1), fades sprites 0..13, 16, 17 out by 1/6, and
   returns 1 once `animFrame` is past 6 and the title plate is done. Otherwise `animFrame` is
   advanced and 0 returned (any other `out` only advances it). */

s32 UiBattleRecordAnimateMenu(UiBattleRecord *self, s32 out)
{
  GfxSprite **sprites;
  GfxSprite *sprite;
  u8 plateDone;
  s32 frame;
  float t;
  float alpha;
  float scaleX;
  float scaleY;
  float posY;
  s32 i;

  if (out == 0) {
    plateDone = UiTitlePlateStep(0);
    frame = self->animFrame;
    if (frame < 6) {
      sprite = ((GfxSprite **)self->base.data)[13];
      alpha = sprite->alpha + 0.16666667f;
      sprite->alpha = alpha;
      ((GfxSprite **)self->base.data)[12]->alpha = alpha;
      ((GfxSprite **)self->base.data)[11]->alpha = alpha;
      ((GfxSprite **)self->base.data)[0]->alpha = alpha;
      ((GfxSprite **)self->base.data)[17]->alpha = alpha;
      ((GfxSprite **)self->base.data)[16]->alpha = alpha;
    } else if (frame < 12) {
      t = (float)(12 - frame);
      scaleX = UiBattleRecordScaleStep(1.2f, 6.0f, t);
      scaleY = UiBattleRecordScaleStep(1.1f, 6.0f, t);
      for (i = 1; i < 11; i++) {
        sprite = ((GfxSprite **)self->base.data)[i];
        sprite->alpha = sprite->alpha + 0.16666667f;
        GfxSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], scaleX, scaleY, 0.0f, false);
      }
    } else {
      sprites = (GfxSprite **)self->base.data;
      posY = sprites[1]->posY;
      sprites[18]->posY = posY;
      ((GfxSprite **)self->base.data)[5]->posY = posY;
      ((GfxSprite **)self->base.data)[6]->posY = posY;
      GfxSpriteSetScaleRotation(((GfxSprite **)self->base.data)[6], 1.15f, 1.15f, 0.0f, false);
      GfxSpriteSetScaleRotation(((GfxSprite **)self->base.data)[18], 1.15f, 1.15f, 0.0f, false);
      GfxSpriteSetScaleRotation(((GfxSprite **)self->base.data)[1], 1.15f, 1.15f, 0.0f, false);
      GfxSpriteSetScaleRotation(((GfxSprite **)self->base.data)[5], 1.15f, 1.15f, 0.0f, false);
      ((GfxSprite **)self->base.data)[5]->flags |= 1;
      if (plateDone != 0) {
        return 1;
      }
    }
  } else if (out == 1) {
    t = (float)self->animFrame;
    UiBattleRecordScaleStep(1.6f, 6.0f, t); /* result unused */
    scaleX = UiBattleRecordScaleStep(1.2f, 6.0f, t);
    scaleY = UiBattleRecordScaleStep(1.1f, 6.0f, t);
    plateDone = UiTitlePlateStep(1);
    ((GfxSprite **)self->base.data)[18]->flags &= ~1u;
    for (i = 0; i < 14; i++) {
      sprite = ((GfxSprite **)self->base.data)[i];
      if (i != 0 && i != 11 && i != 12 && i != 13) {
        GfxSpriteSetScaleRotation(sprite, scaleX, scaleY, 0.0f, false);
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      sprite->alpha = sprite->alpha - 0.16666667f;
    }
    sprite = ((GfxSprite **)self->base.data)[16];
    sprite->alpha = sprite->alpha - 0.16666667f;
    sprite = ((GfxSprite **)self->base.data)[17];
    sprite->alpha = sprite->alpha - 0.16666667f;
    if (self->animFrame >= 7 && plateDone != 0) {
      return 1;
    }
  }
  self->animFrame = self->animFrame + 1;
  return 0;
}
