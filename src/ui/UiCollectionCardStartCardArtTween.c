// bdc 0x08984148 UiCollectionCardStartCardArtTween
#include "bdc.h"

/* Starts the zoom tween (`UiTweenBegin` at scale 0.45000002f (0x3ee66667), flags 3, tweens 0x0d..0x10) of the four
   card art sprites 0x0d..0x10 of `UiCollectionCard`. Zooming in (`out` 0)
   first refreshes them for the current page: an empty slot (0xff) is hidden, an owned card is
   shown with its art (`UiCollectionCardSetCardArt`), and each is put back at its `bobPos` /
   `spriteZ` position. Zooming out only starts the tweens. */

void UiCollectionCardStartCardArtTween(UiCollectionCard *self, u8 out)

{
  int i;
  GfxSprite *sprite;

  if (out == 0) {
    for (i = 0; i < 4; i++) {
      sprite = ((GfxSprite **)self->base.data)[13 + i];
      if (self->slots[self->page * 4 + i] == 0xff) {
        sprite->flags &= ~1u;
      }
      else {
        sprite->flags |= 1;
        UiCollectionCardSetCardArt(&self->base,((GfxSprite **)self->base.data)[13 + i],
                                   self->slots[self->page * 4 + i]);
      }
      ((GfxSprite **)self->base.data)[13 + i]->posX = self->bobPos[i][0];
      ((GfxSprite **)self->base.data)[13 + i]->posY = self->bobPos[i][1];
      ((GfxSprite **)self->base.data)[13 + i]->posZ = self->spriteZ[13 + i];
      UiTweenBegin(0.45000002f,out,((GfxSprite **)self->base.data)[13 + i],&self->tweens[13 + i],3);
    }
  }
  else {
    for (i = 0; i < 4; i++) {
      UiTweenBegin(0.45000002f,out,((GfxSprite **)self->base.data)[13 + i],&self->tweens[13 + i],3);
    }
  }
  return;
}
