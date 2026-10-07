// bdc 0x0891e924 UiHologramGalleryAnimateScrollArrows
#include "bdc.h"

/* Animates the page-scroll arrows of the hologram gallery screen (sprites 0x51..0x54) while
   `pageAnimOn` is set. When the help page changed since the last run, the animation restarts.
   State 0 hides the four arrows, sets `arrowMask` bit 0 if more pages follow and bit 1 past the
   first page, shows the even sprites for bit 0 and the odd ones for bit 1 at their base y
   `arrowY`, then goes to state 1 (or to state 2, no arrows, when the mask is empty). State 1
   bobs them: per sprite the timer ticks once and every 12 ticks the offset `arrowBob` steps
   between 0 and 4; even sprites sit `arrowBob` above, odd ones below their base y. */

void UiHologramGalleryAnimateScrollArrows(UiHologramGallery *self)
{
  GfxSprite **sprites;
  GfxSprite *sprite;
  int page;
  int i;
  u32 k;
  u8 mask;

  if (self->pageAnimOn == 0) {
    return;
  }
  if (self->pageAnimPage != self->helpPage) {
    self->pageAnimPage = self->helpPage;
    self->arrowMask = 0;
    self->arrowBobBack = 0;
    self->arrowBob = 0;
    self->arrowTimer = 0;
    self->arrowState = 0;
  }
  if (self->arrowState == 0) {
    for (i = 0x51; i < 0x55; i++) {
      sprites = (GfxSprite **)self->base.data;
      sprites[i]->flags &= ~1u;
    }
    page = self->helpPage + 1;
    if (page < self->helpPages) {
      self->arrowMask |= 1;
    }
    if (page >= 2) {
      self->arrowMask |= 2;
    }
    if (self->arrowMask == 0) {
      self->arrowState = 2;
      return;
    }
    for (i = 0x51; i < 0x55; i++) {
      k = (u32)(i - 0x51) & 0xff;
      sprite = ((GfxSprite **)self->base.data)[i];
      mask = self->arrowMask;
      if ((k & 1) == 0) {
        if ((mask & 1) != 0) {
          sprite->flags |= 1;
          sprite = ((GfxSprite **)self->base.data)[i];
        }
      }
      else if ((mask & 2) != 0) {
        sprite->flags |= 1;
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      sprite->posY = self->arrowY[k];
    }
    self->arrowState = 1;
    return;
  }
  if (self->arrowState < 2) {
    for (i = 0x51; i < 0x55; i++) {
      self->arrowTimer = self->arrowTimer + 1;
      mask = self->arrowMask;
      if ((int)self->arrowTimer % 12 == 0) {
        if (self->arrowBobBack == 0) {
          self->arrowBob = self->arrowBob + 1;
          if (self->arrowBob >= 4) {
            self->arrowBobBack = 1;
          }
        }
        else {
          self->arrowBob = self->arrowBob - 1;
          if (self->arrowBob == 0) {
            self->arrowBobBack = 0;
          }
        }
      }
      k = (u32)(i - 0x51) & 0xff;
      if ((k & 1) == 0) {
        if ((mask & 1) != 0) {
          ((GfxSprite **)self->base.data)[i]->posY = self->arrowY[k] - (float)self->arrowBob;
        }
      }
      else if ((mask & 2) != 0) {
        ((GfxSprite **)self->base.data)[i]->posY = self->arrowY[k] + (float)self->arrowBob;
      }
    }
  }
}
