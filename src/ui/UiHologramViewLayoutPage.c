// bdc 0x0892a74c UiHologramViewLayoutPage
#include "bdc.h"

/* Lays out the 26 sprites of the current page of the hologram view screen: sets every sprite's
   alpha to 0, gives sprite 0 the page picture (`UiHologramViewSetPagePicture`), shows sprites
   0, 18, 19, 24 and 25 and hides the rest (jump table `0x08a9c290`), then shows the page's extra
   sprites: page 1 sprites 13, 14 and 16, page 2 sprite 17, page 4 sprites 1..12 (scaled to 0.8
   with `UiSpriteSetScaleRotation` and moved 8 left), page 5 sprite 15. */

void UiHologramViewLayoutPage(UiHologramView *self)
{
  GfxSprite *sprite;
  int i;

  for (i = 0; i < 0x1a; i++) {
    ((GfxSprite **)self->base.data)[i]->alpha = 0.0f;
    sprite = ((GfxSprite **)self->base.data)[i];
    if (i == 0) {
      UiHologramViewSetPagePicture(self, sprite, self->page & 0xff);
      sprite = ((GfxSprite **)self->base.data)[i];
    }
    switch (i) {
    case 0:
    case 0x12:
    case 0x13:
    case 0x18:
    case 0x19:
      sprite->flags |= 1;
      break;
    default:
      sprite->flags &= ~1u;
      break;
    }
    if (self->page == 1) {
      if (i < 0xf) {
        if (i >= 0xd) {
          ((GfxSprite **)self->base.data)[i]->flags |= 1;
        }
      }
      else if (i == 0x10) {
        ((GfxSprite **)self->base.data)[i]->flags |= 1;
      }
    }
    if (self->page == 2 && i == 0x11) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
    }
    if (self->page == 4 && i > 0 && i < 0xd) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 0.8f, 0.8f, 0.0f);
      ((GfxSprite **)self->base.data)[i]->posX = ((GfxSprite **)self->base.data)[i]->posX - 8.0f;
    }
    if (self->page == 5 && i == 0xf) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
    }
  }
}
