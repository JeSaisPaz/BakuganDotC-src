// bdc 0x0898ed2c UiCollectionFigureDetailNameDone
#include "bdc.h"

/* Advances the fade tweens of the detail name sprites 0x2b and 0x36 (records `+0x72c`, `+0x8e4`) of
   `UiCollectionFigure` started by
   `UiCollectionFigureStartDetailNameTween`; returns true when at least one of them has finished
   (the per-sprite `UiTweenUpdate` results are summed into a byte counter). The sprite table
   `base.data` is re-read for every call. */

bool UiCollectionFigureDetailNameDone(UiCollectionFigure *self, u8 out)

{
  u8 done = 0;
  s32 i;

  for (i = 0x2b; i < 0x2c; i++) {
    done += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[i],
                          &self->tweens[i], 1);
  }
  for (i = 0x36; i < 0x37; i++) {
    done += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[i],
                          &self->tweens[i], 1);
  }
  return done != 0;
}
