// bdc 0x08805a2c UiNameEntryStartKeyPress
#include "bdc.h"

/* Starts the key-press pop animation of `UiNameEntry` on layout sprite `key`:
   resets that sprite's add colour/matrix, copies it into the highlight sprite (layout sprite 35)
   and sets `keyPressActive` with `keyPressFrame = 0` (see `UiNameEntryAnimateKeyPress`). */

void UiNameEntryStartKeyPress(UiNameEntry *self, s32 key)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;

  sprites[key]->addColor[0] = 0.0f;
  sprites[key]->addColor[1] = 0.0f;
  sprites[key]->addColor[2] = 0.0f;
  sprites[key]->addColor[3] = 1.0f;
  GfxSpriteResetMatrix(sprites[key]);
  sprites = (GfxSprite **)self->base.data;
  GfxSpriteCopy(sprites[key], sprites[35]);
  self->keyPressActive = 1;
  self->keyPressFrame = 0;
}
