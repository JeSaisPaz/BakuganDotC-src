// bdc 0x08983dc0 UiCollectionCardDimPageArrows
#include "bdc.h"

/* Sets the alpha of the left/right page arrows (sprites 0x13, 0x16) of
   `UiCollectionCard` to `alpha` and greys out the left one on page 0 and the
   right one on the last page (0x13). */

void UiCollectionCardDimPageArrows(float alpha, UiScreen *screen)

{
  UiCollectionCard *self = (UiCollectionCard *)screen;
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  GfxSprite *s;

  s = sprites[0x13];
  s->tint[0] = 1.0f;
  s->tint[1] = 1.0f;
  s->tint[2] = 1.0f;
  s->alpha = alpha;
  s = sprites[0x16];
  s->tint[0] = 1.0f;
  s->tint[1] = 1.0f;
  s->tint[2] = 1.0f;
  s->alpha = alpha;
  if (self->page == 0) {
    s = ((GfxSprite **)self->base.data)[0x13];
    s->tint[0] = 0.5f;
    s->tint[1] = 0.5f;
    s->tint[2] = 0.5f;
    s->alpha = alpha;
  }
  if (self->page == 0x13) {
    s = ((GfxSprite **)self->base.data)[0x16];
    s->tint[0] = 0.5f;
    s->tint[1] = 0.5f;
    s->tint[2] = 0.5f;
    s->alpha = alpha;
  }
}
