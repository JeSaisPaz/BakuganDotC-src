// bdc 0x0897add4 UiCollectionSphereDimPageArrows
#include "bdc.h"

/* Sets the alpha of the left/right page arrows (sprites 0x17, 0x1a) of
   `UiCollectionSphere` to `alpha` and greys out (0.5) the left one on the
   first page and the right one on the last page of the category (categories 0..1: last page from
   the entry count, 6 per page; category 2: page 5). */

static void SetArrowColor(GfxSprite *arrow, float shade, float alpha)
{
  arrow->tint[0] = shade;
  arrow->tint[1] = shade;
  arrow->tint[2] = shade;
  arrow->alpha = alpha;
}

void UiCollectionSphereDimPageArrows(float alpha, UiScreen *screen)
{
  UiCollectionSphere *self = (UiCollectionSphere *)screen;
  s8 category;

  SetArrowColor(((GfxSprite **)self->base.data)[0x17], 1.0f, alpha);
  SetArrowColor(((GfxSprite **)self->base.data)[0x1a], 1.0f, alpha);
  if (self->page == 0) {
    SetArrowColor(((GfxSprite **)self->base.data)[0x17], 0.5f, alpha);
  }
  category = self->category;
  if (category < 2) {
    if (category >= 0 && self->page == (self->entryCount + 5) / 6 - 1) {
      SetArrowColor(((GfxSprite **)self->base.data)[0x1a], 0.5f, alpha);
    }
  } else if (category < 3 && self->page == 5) {
    SetArrowColor(((GfxSprite **)self->base.data)[0x1a], 0.5f, alpha);
  }
}
