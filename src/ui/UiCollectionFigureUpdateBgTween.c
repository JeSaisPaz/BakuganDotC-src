// bdc 0x0898c68c UiCollectionFigureUpdateBgTween
#include "bdc.h"

/* Advances the background sprite tweens 0x23..0x27 of `UiCollectionFigure`
   (`UiTweenUpdate` mode 1, 16 frames); returns true once any of them has finished
   (they share one duration, so in practice all finish together). */

bool UiCollectionFigureUpdateBgTween(UiCollectionFigure *self, u8 out)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  u8 finished = 0;
  int i;

  for (i = 0x23; i < 0x28; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, sprites[i], &self->tweens[i], 1);
  }
  return finished != 0;
}
