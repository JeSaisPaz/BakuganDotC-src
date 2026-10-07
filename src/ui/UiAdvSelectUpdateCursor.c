// bdc 0x08919c44 UiAdvSelectUpdateCursor
#include "bdc.h"

/* Moves the selection highlight of the adventure partner-select screen (`UiAdvSelectCtor`, task
   376): resets the glow (`UiCursorGlowReset`), `focusZoom` and the cursor pulse (`tweens[17]`,
   `UiPulseReset`), shows the cursor sprite 17 centred (`GfxSpriteCenterPivot`) at scale 1,
   alpha 1 and grey add colour (0.3, 0.3, 0.3, 1) over the focused candidate sprite `5 + cursor`
   (its X/Y, its own saved Z), and restarts the pulse with ghost sprite 39 (`tweens[39]`,
   `UiPulseInit`). Then resets sprites 5..10, 11..16 and 18..23 to scale 1 / angle 0 at their
   saved Z; sprites 18..23 are also moved to the saved position of sprite `i - 13` plus
   `candidateOffsetX/Y`. */

void UiAdvSelectUpdateCursor(UiAdvSelect *self)
{
  GfxSprite *sprite;
  s32 i;

  UiCursorGlowReset();
  self->focusZoom = 0.0f;
  UiPulseReset((UiPulse *)&self->tweens[17]);
  sprite = ((GfxSprite **)self->base.data)[17];
  sprite->flags |= 1;
  GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[17]);
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[17], 1.0f, 1.0f, 0.0f);
  ((GfxSprite **)self->base.data)[17]->alpha = 1.0f;
  sprite = ((GfxSprite **)self->base.data)[17];
  sprite->addColor[3] = 1.0f;
  sprite->addColor[0] = 0.3f;
  sprite->addColor[1] = 0.3f;
  sprite->addColor[2] = 0.3f;
  ((GfxSprite **)self->base.data)[17]->posX =
      ((GfxSprite **)self->base.data)[5 + self->cursor]->posX;
  ((GfxSprite **)self->base.data)[17]->posY =
      ((GfxSprite **)self->base.data)[5 + self->cursor]->posY;
  ((GfxSprite **)self->base.data)[17]->posZ = self->spriteZ[17];
  UiPulseInit(((GfxSprite **)self->base.data)[17], ((GfxSprite **)self->base.data)[39],
              (UiPulse *)&self->tweens[39]);

  for (i = 5; i < 11; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }
  for (i = 11; i < 17; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }
  for (i = 18; i < 24; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
    ((GfxSprite **)self->base.data)[i]->posX = self->spritePos[i - 13][0] + self->candidateOffsetX;
    ((GfxSprite **)self->base.data)[i]->posY = self->spritePos[i - 13][1] + self->candidateOffsetY;
  }
}
