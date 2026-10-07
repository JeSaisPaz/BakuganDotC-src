// bdc 0x0892f260 UiBakuganSelectShowCurrentMark
#include "bdc.h"

/* Shows (or hides) the "current Bakugan" marker sprite 0x18 of the Bakugan select screen
   (`UiBakuganSelectCtor`). Showing makes it visible (flags bit 0), sets layer mask 2 and full
   alpha, moves it onto the grid cell sprite (0x1a + position) of the profile's current Bakugan
   (`currentPos`, converted with `UiBakuganListOrder`) at depth spriteZ[0x18] and resets its
   scale/rotation to 1/1/0. Hiding only clears flags bit 0. */

void UiBakuganSelectShowCurrentMark(UiBakuganSelect *self, bool show)
{
  u8 pos;
  GfxSprite **sprites;

  if (show) {
    pos = UiBakuganListOrder(self, false, self->currentPos);
    sprites = (GfxSprite **)self->base.data;
    sprites[0x18]->flags |= 1;
    sprites[0x18]->layerMask = 2;
    sprites[0x18]->alpha = 1.0f;
    sprites[0x18]->posX = sprites[0x1a + pos]->posX;
    sprites[0x18]->posY = sprites[0x1a + pos]->posY;
    sprites[0x18]->posZ = self->spriteZ[0x18];
    UiSpriteSetScaleRotation(sprites[0x18], 1.0f, 1.0f, 0.0f);
  }
  else {
    sprites = (GfxSprite **)self->base.data;
    sprites[0x18]->flags &= ~1u;
  }
}
