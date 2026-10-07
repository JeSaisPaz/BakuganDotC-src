// bdc 0x08953df0 UiBattleRuleSelectResetSubCursor
#include "bdc.h"

/* Resets the sub-option cursor of `UiBattleRuleSelect`: clears
   `subFocusZoom`, restores scale 1 and the saved depth `spriteHomeZ` of sprites 0x0b–0x13, resets
   the pulse in tween slot 21 (`UiPulseReset`), then makes the cursor sprite 0x15 visible on layer
   2, centred, scale 1, alpha 1, add-colour (0.3, 0.3, 0.3, 1), placed over sub-option sprite
   0x11 + `subCursor` at its saved depth, and starts its pulse (`UiPulseInit`, ghost sprite 0x1c,
   tween slot 28). */

void UiBattleRuleSelectResetSubCursor(UiBattleRuleSelect *self)
{
  GfxSprite **sprites;
  GfxSprite *cursor;
  s32 i;

  self->subFocusZoom = 0.0f;
  for (i = 0xb; i < 0x11; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteHomeZ[i];
  }
  for (i = 0x11; i < 0x14; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteHomeZ[i];
  }
  UiPulseReset((UiPulse *)&self->tweens[21]);

  ((GfxSprite **)self->base.data)[0x15]->layerMask = 2;
  cursor = ((GfxSprite **)self->base.data)[0x15];
  cursor->flags |= 1;
  GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[0x15]);
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[0x15], 1.0f, 1.0f, 0.0f);
  ((GfxSprite **)self->base.data)[0x15]->alpha = 1.0f;
  cursor = ((GfxSprite **)self->base.data)[0x15];
  cursor->addColor[3] = 1.0f;
  cursor->addColor[0] = 0.3f;
  cursor->addColor[1] = 0.3f;
  cursor->addColor[2] = 0.3f;

  sprites = (GfxSprite **)self->base.data;
  sprites[0x15]->posX = sprites[0x11 + self->subCursor]->posX;
  sprites = (GfxSprite **)self->base.data;
  sprites[0x15]->posY = sprites[0x11 + self->subCursor]->posY;
  ((GfxSprite **)self->base.data)[0x15]->posZ = self->spriteHomeZ[0x15];
  sprites = (GfxSprite **)self->base.data;
  UiPulseInit(sprites[0x15], sprites[0x1c], (UiPulse *)&self->tweens[28]);
}
