// bdc 0x089853c4 UiCollectionCardUpdateDetailNameTween
#include "bdc.h"

/* Advances the detail name tweens of `UiCollectionCard` (sprites 39 and 50)
   started by `UiCollectionCardStartDetailNameTween`; returns true when any of them reports
   finished (they run in lockstep, so in practice: all done). */

bool UiCollectionCardUpdateDetailNameTween(UiCollectionCard *self, u8 out)
{
  u8 done = 0;
  int i;

  for (i = 39; i < 40; i++)
    done += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[i],
                          &self->tweens[i], 1);
  for (i = 50; i < 51; i++)
    done += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[i],
                          &self->tweens[i], 1);
  return done != 0;
}
