// bdc 0x08977114 UiCollectionMenuResetCursor
#include "bdc.h"

/* Resets the entry buttons and cursor of the collection top menu (`UiCollectionMenu`):
   clears the cursor glow and `pulse`; on page 0 the main-page sprites 3..5 and 10..11 go back to
   scale 1 and their saved depth (`spriteZ[]`) while sprites 6..9 and 1 are hidden; on the sub-page
   sprites 3..5 and 10..11 are hidden, 6..9 reset and sprite 1 shown. Then the cursor sprite 12 is
   reset (pulse `tweens[12]`, shown, centred, scale 1, grey add-colour 0.3) and moved onto the
   selected button `13 + (&selMain)[page]`, the highlight ghost 25 is primed (`UiPulseInit`), and
   buttons 13..17 are reset with the selected one highlighted (`"waku_1_a"` frame). */

void UiCollectionMenuResetCursor(UiCollectionMenu *self)

{
  GfxSprite *sprite;
  s32 i;

  UiCursorGlowReset();
  self->pulse = 0.0f;
  if (self->page == 0) {
    for (i = 3; i < 6; i++) {
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
      ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
    }
    for (i = 10; i < 12; i++) {
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
      ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
    }
    for (i = 6; i < 10; i++) {
      ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
    }
    ((GfxSprite **)self->base.data)[1]->flags &= ~1u;
  }
  else {
    for (i = 3; i < 6; i++) {
      ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
    }
    for (i = 10; i < 12; i++) {
      ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
    }
    for (i = 6; i < 10; i++) {
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
      ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
    }
    ((GfxSprite **)self->base.data)[1]->flags |= 1;
  }
  UiPulseReset((UiPulse *)&self->tweens[12]);
  ((GfxSprite **)self->base.data)[12]->flags |= 1;
  GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[12]);
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[12], 1.0f, 1.0f, 0.0f);
  ((GfxSprite **)self->base.data)[12]->alpha = 1.0f;
  sprite = ((GfxSprite **)self->base.data)[12];
  sprite->addColor[3] = 1.0f;
  sprite->addColor[0] = 0.3f;
  sprite->addColor[1] = 0.3f;
  sprite->addColor[2] = 0.3f;
  ((GfxSprite **)self->base.data)[12]->posZ = self->spriteZ[12];
  ((GfxSprite **)self->base.data)[12]->posX =
      ((GfxSprite **)self->base.data)[13 + (&self->selMain)[self->page]]->posX;
  ((GfxSprite **)self->base.data)[12]->posY =
      ((GfxSprite **)self->base.data)[13 + (&self->selMain)[self->page]]->posY;
  UiPulseInit(((GfxSprite **)self->base.data)[12], ((GfxSprite **)self->base.data)[25],
              (UiPulse *)&self->tweens[25]);
  for (i = 13; i < 18; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
    sprite = ((GfxSprite **)self->base.data)[i];
    if (i - 13 == (&self->selMain)[self->page]) {
      sprite->addColor[0] = 0.3f;
      sprite->addColor[1] = 0.3f;
      sprite->addColor[2] = 0.3f;
      sprite->addColor[3] = 1.0f;
      UiCollectionMenuSetButtonFrame(self, ((GfxSprite **)self->base.data)[i], 1);
    }
    else {
      sprite->addColor[0] = 0.0f;
      sprite->addColor[1] = 0.0f;
      sprite->addColor[2] = 0.0f;
      sprite->addColor[3] = 1.0f;
      UiCollectionMenuSetButtonFrame(self, ((GfxSprite **)self->base.data)[i], 0);
    }
  }
  return;
}
