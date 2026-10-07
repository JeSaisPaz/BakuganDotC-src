// bdc 0x089ae778 UiPauseSettingsRefreshCursor
#include "bdc.h"

/* Updates the highlight of every menu row of `UiPauseSettings` for the cursor:
   clears `buttonScale`; for the first `itemCount` rows, rows 0..3 (toggle/label sprites 0x1f+row)
   go through `UiPauseSettingsSetToggleTexture` and rows 4..6 (button sprites 0x2e+row, with their
   second sprite 0x31+row) through `UiPauseSettingsSetButtonTexture`, each with
   `UiSpriteSetHighlight` on (row == cursor) or off, a clear of its 0x28-byte slot (`plateSlides[row]`
   resp. `buttonTweens[row - 4]`) and full white tint and alpha; the button sprites also get scale
   1 / angle 0 back and their base Z from `spriteZ`. When the cursor is on a button row (>= 4) it
   clears `highlightPulse`, shows the cursor frame (sprite 0x38, layer 2) centred over the selected
   button with a 0.3 colour-add and starts its pulse with ghost sprite 0x3f (`UiPulseInit`,
   `buttonPulse`); otherwise hides sprites 0x38 and 0x3f. Finally resets the cursor glow
   (`UiCursorGlowReset`). */

void UiPauseSettingsRefreshCursor(UiPauseSettings *self)
{
  GfxSprite **sprites;
  GfxSprite *sprite;
  int i;

  self->buttonScale = 0.0f;
  for (i = 0; i < self->itemCount; i++) {
    switch (i) {
    case 0:
    case 1:
    case 2:
    case 3:
      sprites = (GfxSprite **)self->base.data;
      if (i == self->cursor) {
        UiPauseSettingsSetToggleTexture(self, sprites[0x1f + i], 1);
        UiSpriteSetHighlight(((GfxSprite **)self->base.data)[0x1f + i], 1);
      } else {
        UiPauseSettingsSetToggleTexture(self, sprites[0x1f + i], 0);
        UiSpriteSetHighlight(((GfxSprite **)self->base.data)[0x1f + i], 0);
      }
      memset(&self->plateSlides[i], 0, 0x28);
      sprite = ((GfxSprite **)self->base.data)[0x1f + i];
      sprite->tint[0] = 1.0f;
      sprite->tint[1] = 1.0f;
      sprite->tint[2] = 1.0f;
      sprite->alpha = 1.0f;
      break;
    case 4:
    case 5:
    case 6:
      sprites = (GfxSprite **)self->base.data;
      if (i == self->cursor) {
        UiPauseSettingsSetButtonTexture(self, sprites[0x2e + i], 1);
        UiSpriteSetHighlight(((GfxSprite **)self->base.data)[0x2e + i], 1);
      } else {
        UiPauseSettingsSetButtonTexture(self, sprites[0x2e + i], 0);
        UiSpriteSetHighlight(((GfxSprite **)self->base.data)[0x2e + i], 0);
      }
      memset(&self->buttonTweens[i - 4], 0, 0x28);
      sprite = ((GfxSprite **)self->base.data)[0x2e + i];
      sprite->tint[0] = 1.0f;
      sprite->tint[1] = 1.0f;
      sprite->tint[2] = 1.0f;
      sprite->alpha = 1.0f;
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[0x2e + i], 1.0f, 1.0f, 0.0f);
      ((GfxSprite **)self->base.data)[0x2e + i]->posZ = self->spriteZ[0x2e + i];
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[0x31 + i], 1.0f, 1.0f, 0.0f);
      ((GfxSprite **)self->base.data)[0x31 + i]->posZ = self->spriteZ[0x31 + i];
      break;
    }
  }
  if (self->cursor < 4) {
    ((GfxSprite **)self->base.data)[0x38]->flags &= ~1u;
    ((GfxSprite **)self->base.data)[0x3f]->flags &= ~1u;
  } else {
    memset(self->highlightPulse, 0, 0x28);
    ((GfxSprite **)self->base.data)[0x38]->layerMask = 2;
    ((GfxSprite **)self->base.data)[0x38]->flags |= 1;
    GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[0x38]);
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[0x38], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[0x38]->alpha = 1.0f;
    sprite = ((GfxSprite **)self->base.data)[0x38];
    sprite->addColor[3] = 1.0f;
    sprite->addColor[0] = 0.3f;
    sprite->addColor[1] = 0.3f;
    sprite->addColor[2] = 0.3f;
    sprites = (GfxSprite **)self->base.data;
    sprites[0x38]->posX = sprites[0x2e + self->cursor]->posX;
    sprites = (GfxSprite **)self->base.data;
    sprites[0x38]->posY = sprites[0x2e + self->cursor]->posY;
    ((GfxSprite **)self->base.data)[0x38]->posZ = self->spriteZ[0x38];
    sprites = (GfxSprite **)self->base.data;
    UiPulseInit(sprites[0x38], sprites[0x3f], &self->buttonPulse);
  }
  UiCursorGlowReset();
}
