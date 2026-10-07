// bdc 0x0898ea10 UiCollectionFigureDetailPanelDone
#include "bdc.h"

/* Advances the fade tweens of the detail panel sprites 0x29/0x2a and 0x2c/0x2d (records `+0x6dc`,
   `+0x754`) of `UiCollectionFigure` started by
   `UiCollectionFigureStartDetailPanelTween`; returns true when finished. */

bool UiCollectionFigureDetailPanelDone(UiCollectionFigure *self, u8 out)

{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  u8 active = 0;
  s32 i;

  for (i = 0x29; i < 0x2b; i++) {
    active += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, sprites[i], &self->tweens[i], 1);
  }
  for (i = 0x2c; i < 0x2e; i++) {
    active += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, sprites[i], &self->tweens[i], 1);
  }
  return active != 0;
}
