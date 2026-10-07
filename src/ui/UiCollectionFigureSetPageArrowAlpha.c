// bdc 0x0898cf74 UiCollectionFigureSetPageArrowAlpha
#include "bdc.h"

/* Sets the alpha of the two page-arrow sprites (data `+0x54` left, `+0x60` right) of
   `UiCollectionFigure` to `alpha`, dimming (tint 0.5) the left arrow on
   the first page and the right arrow on the last page (`(count +0xe84 + 5) / 6 - 1`). */

void UiCollectionFigureSetPageArrowAlpha(UiCollectionFigure *self, float alpha)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  GfxSprite *spr;
  int count;

  spr = sprites[0x54 / 4];
  spr->tint[0] = 1.0f;
  spr->tint[1] = 1.0f;
  spr->tint[2] = 1.0f;
  spr->alpha = alpha;
  spr = sprites[0x60 / 4];
  spr->tint[0] = 1.0f;
  spr->tint[1] = 1.0f;
  spr->tint[2] = 1.0f;
  spr->alpha = alpha;
  if (self->page == 0) {
    spr = sprites[0x54 / 4];
    spr->tint[0] = 0.5f;
    spr->tint[1] = 0.5f;
    spr->tint[2] = 0.5f;
    spr->alpha = alpha;
  }
  count = self->entryCount;
  if ((int)self->page == (count + 5) / 6 - 1) {
    spr = sprites[0x60 / 4];
    spr->tint[0] = 0.5f;
    spr->tint[1] = 0.5f;
    spr->tint[2] = 0.5f;
    spr->alpha = alpha;
  }
}
