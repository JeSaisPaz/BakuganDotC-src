// bdc 0x0894c738 UiBattleRecordMenuInput
#include "bdc.h"

/* Cursor input of the category menu of the battle-record screen (task 3005, `UiBattleRecordCtor`;
   per-Bakugan win/loss statistics from the save profile, layout package `"data/2d/%s/record.lzs"`):
   up/down (pad repeat bits 0x10/0x40, sound 1) cycle the selection `mode` through the four entries
   (wrapping), move the cursor sprites to the new entry and enlarge it to 1.15x, then restart the
   highlight; every call then runs one step of the 16-frame cursor pulse (sprite 18 grows and fades,
   sprite 5 brightens) or resets the highlight once it is over. */

void UiBattleRecordMenuInput(UiBattleRecord *self)
{
  PadState *pad = self->base.pad;
  GfxSprite **sprites;
  GfxSprite *sprite;
  s32 delta;
  s32 frame;
  float posY;
  float scale;
  float glow;

  if (pad->repeat & 0x10) {
    delta = 3;
  } else if (pad->repeat & 0x40) {
    delta = 1;
  } else {
    delta = 0;
  }

  if (delta != 0) {
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 1, 0, 0);
    }
    GfxSpriteResetMatrix(((GfxSprite **)self->base.data)[self->mode + 1]);
    sprites = (GfxSprite **)self->base.data;
    self->mode = (self->mode + delta) % 4;
    posY = sprites[self->mode + 1]->posY;
    sprites[18]->posY = posY;
    ((GfxSprite **)self->base.data)[5]->posY = posY;
    ((GfxSprite **)self->base.data)[6]->posY = posY;
    GfxSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->mode + 1], 1.15f, 1.15f, 0.0f, false);
    GfxSpriteSetScaleRotation(((GfxSprite **)self->base.data)[6], 1.15f, 1.15f, 0.0f, false);
    GfxSpriteSetScaleRotation(((GfxSprite **)self->base.data)[18], 1.15f, 1.15f, 0.0f, false);
    UiBattleRecordResetMenuHighlight(self);
  }

  frame = self->animFrame;
  if (frame < 16) {
    scale = (float)(frame + 1) * 0.100000024f * 0.0625f + 1.15f;
    glow = (float)frame * 0.021875f;
    GfxSpriteSetScaleRotation(((GfxSprite **)self->base.data)[18], scale, scale, 0.0f, false);
    sprite = ((GfxSprite **)self->base.data)[5];
    sprite->addColor[0] = glow;
    sprite->addColor[1] = glow;
    sprite->addColor[2] = glow;
    sprite->addColor[3] = 1.0f;
    ((GfxSprite **)self->base.data)[18]->alpha -= 0.075f;
    self->animFrame = self->animFrame + 1;
  } else {
    UiBattleRecordResetMenuHighlight(self);
  }
}
