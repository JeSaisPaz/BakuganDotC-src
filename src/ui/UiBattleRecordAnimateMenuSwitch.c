// bdc 0x0894c2f4 UiBattleRecordAnimateMenuSwitch
#include "bdc.h"

/* Open (`out` = 0) / close (`out` = 1) animation of the category menu of the battle-record screen
   (task 3005, `UiBattleRecordCtor`; per-Bakugan win/loss statistics from the save profile, layout
   package `"data/2d/%s/record.lzs"`) used when switching to a statistics list and back; one step
   per call. Open: for 6 frames sprites 1..10 fade in by 1/6 and shrink from (1.2, 1.1) to 1; on the
   7th call the cursor sprites (18, 5, 6) move to the selected entry, it and sprites 5/6/18 are set to
   1.15x, sprite 5 is made visible and 1 is returned (`animFrame` not advanced). Close: hides sprite
   18, fades sprites 1..10 out by 1/6 and grows them towards (1.2, 1.1); returns 1 once `animFrame`
   is past 6. Any other `out` only advances `animFrame`. Returns 0 while running. */

s32 UiBattleRecordAnimateMenuSwitch(UiBattleRecord *self, s32 out)
{
  s32 frame = self->animFrame;
  GfxSprite *sprite;
  float t;
  float scaleX;
  float scaleY;
  float posY;
  s32 i;

  if (out == 0) {
    if (frame < 6) {
      t = (float)(6 - frame);
      scaleX = UiBattleRecordScaleStep(1.2f, 6.0f, t);
      scaleY = UiBattleRecordScaleStep(1.1f, 6.0f, t);
      for (i = 1; i < 11; i++) {
        sprite = ((GfxSprite **)self->base.data)[i];
        sprite->alpha = sprite->alpha + 0.16666667f;
        GfxSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], scaleX, scaleY, 0.0f, false);
      }
      frame = self->animFrame;
    } else {
      posY = ((GfxSprite **)self->base.data)[self->mode + 1]->posY;
      ((GfxSprite **)self->base.data)[18]->posY = posY;
      ((GfxSprite **)self->base.data)[5]->posY = posY;
      ((GfxSprite **)self->base.data)[6]->posY = posY;
      GfxSpriteSetScaleRotation(((GfxSprite **)self->base.data)[6], 1.15f, 1.15f, 0.0f, false);
      GfxSpriteSetScaleRotation(((GfxSprite **)self->base.data)[18], 1.15f, 1.15f, 0.0f, false);
      GfxSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->mode + 1], 1.15f, 1.15f, 0.0f, false);
      GfxSpriteSetScaleRotation(((GfxSprite **)self->base.data)[5], 1.15f, 1.15f, 0.0f, false);
      ((GfxSprite **)self->base.data)[5]->flags |= 1;
      return 1;
    }
  } else if (out == 1) {
    t = (float)frame;
    UiBattleRecordScaleStep(1.6f, 6.0f, t); /* result unused */
    scaleX = UiBattleRecordScaleStep(1.2f, 6.0f, t);
    scaleY = UiBattleRecordScaleStep(1.1f, 6.0f, t);
    ((GfxSprite **)self->base.data)[18]->flags &= ~1u;
    for (i = 0; i < 14; i++) {
      if (i == 0 || i == 11 || i == 12 || i == 13) {
        continue;
      }
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->alpha = sprite->alpha - 0.16666667f;
      GfxSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], scaleX, scaleY, 0.0f, false);
    }
    frame = self->animFrame;
    if (frame >= 7) {
      return 1;
    }
  }
  self->animFrame = frame + 1;
  return 0;
}
