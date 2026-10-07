// bdc 0x0898e094 UiCollectionFigureStartPageArrowAnim
#include "bdc.h"

/* Starts the press animation (`UiFlashStartRgb`, 4 frames) of the page arrow on the side of the page
   change (`+0xe7b`, sprite data `[side * 3 + 0x12]`) of
   `UiCollectionFigure`; `UiCollectionFigureAnimatePageArrow` then plays
   it. */

void UiCollectionFigureStartPageArrowAnim(UiCollectionFigure *self)

{
  GfxSprite **table = (GfxSprite **)self->base.data;

  UiFlashStartRgb(4.0f, table[self->pageDir * 3 + 0x12], 1, 0, 0, 1, 1);
}
