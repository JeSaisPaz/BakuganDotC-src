// bdc 0x089850ac UiCollectionCardUpdateDetailPanelTween
#include "bdc.h"

/* Advances the detail panel tweens of `UiCollectionCard` (sprites 0x25/0x26
   and 0x28/0x29) started by `UiCollectionCardStartDetailPanelTween`; returns true when any of them
   reports finished (they run in lockstep, so in practice: all done). */

bool UiCollectionCardUpdateDetailPanelTween(UiCollectionCard *self, u8 out)
{
  u8 done = 0;
  int i;

  for (i = 0x25; i < 0x27; i++)
    done += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[i],
                          &self->tweens[i], 1);
  for (i = 0x28; i < 0x2a; i++)
    done += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[i],
                          &self->tweens[i], 1);
  return done != 0;
}
