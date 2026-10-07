// bdc 0x08985a2c UiCollectionCardUpdateAltPanelTween
#include "bdc.h"

/* Advances the zoom-view panel tweens of `UiCollectionCard` (sprites
   0x36/0x37 and 0x3a/0x3b) started by `UiCollectionCardStartAltPanelTween`; returns true when any
   of them reports finished (they run in lockstep, so in practice: all done). */

bool UiCollectionCardUpdateAltPanelTween(UiCollectionCard *self, u8 out)
{
  u8 done = 0;
  int i;

  for (i = 0x36; i < 0x38; i++)
    done += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[i],
                          &self->tweens[i], 1);
  for (i = 0x3a; i < 0x3c; i++)
    done += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[i],
                          &self->tweens[i], 1);
  return done != 0;
}
