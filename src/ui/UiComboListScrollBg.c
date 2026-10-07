// bdc 0x089b4198 UiComboListScrollBg
#include "bdc.h"

/* Animates the background of `UiComboList` every frame: scrolls the two tiled
   sprites 73 and 74 (`data+0x124/0x128`) up by 0.5 px, wrapping from Y <= -272 to 272, and every 16
   frames (counter `bgFrame`) mirrors sprite 75 (`GfxSpriteFlipU`). */

void UiComboListScrollBg(UiComboList *self)
{
  GfxSprite **sprites = (GfxSprite **)(self->base).data;
  GfxSprite *a = sprites[73];
  GfxSprite *b;

  a->posY = a->posY - 0.5f;
  sprites[74]->posY = sprites[74]->posY - 0.5f;
  if (a->posY <= -272.0f) {
    a->posY = 272.0f;
  }
  b = sprites[74];
  if (b->posY <= -272.0f) {
    b->posY = 272.0f;
  }
  self->bgFrame = (self->bgFrame + 1) % 16;
  if (self->bgFrame == 0) {
    GfxSpriteFlipU(sprites[75]);
  }
}
