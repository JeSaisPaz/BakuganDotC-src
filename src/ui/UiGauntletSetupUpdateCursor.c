// bdc 0x08936944 UiGauntletSetupUpdateCursor
#include "bdc.h"

/* Moves the selection highlight of the gauntlet setup screen (task 373, `maybe_UiScreen373Ctor`;
   sprite table `base.data`). Resets the cursor glow and `focusZoom`, then:
   - focusArea 0 (slot area): resets pulse 18, shows cursor sprite 18 (centred pivot, unit scale, alpha 1,
     grey add-colour 0.3) over slot sprite `14 + item` at `spriteZ[18]`, starts pulse 59 with ghost sprite 59,
     and hides sprite 39;
   - otherwise (OK button): hides sprite 18, resets pulse 39, shows sprite 39 the same way at `spriteZ[39]`
     and starts pulse 59 on it.
   Finally restores unit scale and the rest Z (`spriteZ[i]`) of sprites 38, 47, 14-17, 42-45, 34-37, 50-53,
   24-27, 10-13 and 20-23; sprite 38 (the OK button) also gets its texture and add-colour from
   `UiGauntletSetupSetOkButtonTexture`: unlit / black in the slot area, lit / grey 0.3 otherwise. */

void UiGauntletSetupUpdateCursor(UiGauntletSetup *self)
{
  int i;

  UiCursorGlowReset();
  self->focusZoom = 0.0f;
  if (self->focusArea == 0) {
    UiPulseReset((UiPulse *)&self->tweens[18]);
    ((GfxSprite **)self->base.data)[18]->flags |= 1;
    ((GfxSprite **)self->base.data)[18]->layerMask = 2;
    GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[18]);
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[18], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[18]->alpha = 1.0f;
    ((GfxSprite **)self->base.data)[18]->addColor[0] = 0.3f;
    ((GfxSprite **)self->base.data)[18]->addColor[1] = 0.3f;
    ((GfxSprite **)self->base.data)[18]->addColor[2] = 0.3f;
    ((GfxSprite **)self->base.data)[18]->addColor[3] = 1.0f;
    ((GfxSprite **)self->base.data)[18]->posX =
        ((GfxSprite **)self->base.data)[self->item + 14]->posX;
    ((GfxSprite **)self->base.data)[18]->posY =
        ((GfxSprite **)self->base.data)[self->item + 14]->posY;
    ((GfxSprite **)self->base.data)[18]->posZ = self->spriteZ[18];
    UiPulseInit(((GfxSprite **)self->base.data)[18], ((GfxSprite **)self->base.data)[59],
                (UiPulse *)&self->tweens[59]);
    ((GfxSprite **)self->base.data)[39]->flags &= ~1u;
  } else {
    ((GfxSprite **)self->base.data)[18]->flags &= ~1u;
    UiPulseReset((UiPulse *)&self->tweens[39]);
    ((GfxSprite **)self->base.data)[39]->flags |= 1;
    ((GfxSprite **)self->base.data)[39]->layerMask = 2;
    GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[39]);
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[39], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[39]->alpha = 1.0f;
    ((GfxSprite **)self->base.data)[39]->addColor[0] = 0.3f;
    ((GfxSprite **)self->base.data)[39]->addColor[1] = 0.3f;
    ((GfxSprite **)self->base.data)[39]->addColor[2] = 0.3f;
    ((GfxSprite **)self->base.data)[39]->addColor[3] = 1.0f;
    ((GfxSprite **)self->base.data)[39]->posZ = self->spriteZ[39];
    UiPulseInit(((GfxSprite **)self->base.data)[39], ((GfxSprite **)self->base.data)[59],
                (UiPulse *)&self->tweens[59]);
  }

  for (i = 38; i < 39; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
    if (self->focusArea == 0) {
      UiGauntletSetupSetOkButtonTexture(self, ((GfxSprite **)self->base.data)[i], 0);
      ((GfxSprite **)self->base.data)[i]->addColor[0] = 0.0f;
      ((GfxSprite **)self->base.data)[i]->addColor[1] = 0.0f;
      ((GfxSprite **)self->base.data)[i]->addColor[2] = 0.0f;
      ((GfxSprite **)self->base.data)[i]->addColor[3] = 1.0f;
    } else {
      UiGauntletSetupSetOkButtonTexture(self, ((GfxSprite **)self->base.data)[i], 1);
      ((GfxSprite **)self->base.data)[i]->addColor[0] = 0.3f;
      ((GfxSprite **)self->base.data)[i]->addColor[1] = 0.3f;
      ((GfxSprite **)self->base.data)[i]->addColor[2] = 0.3f;
      ((GfxSprite **)self->base.data)[i]->addColor[3] = 1.0f;
    }
  }
  for (i = 47; i < 48; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }
  for (i = 14; i < 18; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }
  for (i = 42; i < 46; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }
  for (i = 34; i < 38; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }
  for (i = 50; i < 54; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }
  for (i = 24; i < 28; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }
  for (i = 10; i < 14; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }
  for (i = 20; i < 24; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }
}
