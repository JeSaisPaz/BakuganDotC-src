// bdc 0x08974b24 UiCollectionMenuUpdateButtonSlide
#include "bdc.h"

/* Advances the slide tweens (`UiTweenUpdate` mode 5, duration `+0x54c`) of the entry buttons of
   the current page of the collection top menu (task 311, `maybe_UiScreen311Ctor`; page `+0x503`
   0 = main entries, 1 = sub-page). Each tween i drives sprite i of the screen's sprite table
   (`base.data`). Page 0 updates tweens 3..5, 13..17, 10..11; page 1 updates 1, 6..9, 13..16.
   Returns true once any of them has finished (the `UiTweenUpdate` results are summed into a
   `u8` and tested for non-zero). */

bool UiCollectionMenuUpdateButtonSlide(UiCollectionMenu *self, u8 out)
{
  GfxSprite **sprites;
  u8 done = 0;
  int i;

  if (self->page == 0) {
    for (i = 3; i < 6; i++) {
      sprites = (GfxSprite **)self->base.data;
      done += UiTweenUpdate(1.0f, 1.0f, self->slideDuration, out, sprites[i], &self->tweens[i], 5);
    }
    for (i = 13; i < 18; i++) {
      sprites = (GfxSprite **)self->base.data;
      done += UiTweenUpdate(1.0f, 1.0f, self->slideDuration, out, sprites[i], &self->tweens[i], 5);
    }
    for (i = 10; i < 12; i++) {
      sprites = (GfxSprite **)self->base.data;
      done += UiTweenUpdate(1.0f, 1.0f, self->slideDuration, out, sprites[i], &self->tweens[i], 5);
    }
  }
  else {
    for (i = 1; i < 2; i++) {
      sprites = (GfxSprite **)self->base.data;
      done += UiTweenUpdate(1.0f, 1.0f, self->slideDuration, out, sprites[i], &self->tweens[i], 5);
    }
    for (i = 6; i < 10; i++) {
      sprites = (GfxSprite **)self->base.data;
      done += UiTweenUpdate(1.0f, 1.0f, self->slideDuration, out, sprites[i], &self->tweens[i], 5);
    }
    for (i = 13; i < 17; i++) {
      sprites = (GfxSprite **)self->base.data;
      done += UiTweenUpdate(1.0f, 1.0f, self->slideDuration, out, sprites[i], &self->tweens[i], 5);
    }
  }
  return done != 0;
}
