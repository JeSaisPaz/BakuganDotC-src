// bdc 0x0897aa34 UiCollectionSphereStartFrameTween
#include "bdc.h"

/* Starts the zoom tween (UiTweenBegin flags 3, scale 1.0) of the frame/title sprites 0x21..0x24
   (data sprite table, tweens 0x21..0x24) of the sphere (Bakugan figure) collection screen
   (task 312, `maybe_UiScreen312Ctor`; `out` = 0 in, 1 out). Order: 0x21, then 0x23/0x24, then
   0x22. When tweening in, sprites 0x21 and 0x22 are made visible first and the page label is
   refreshed (`UiCollectionSphereSetPageNumber`) before the page digits 0x23/0x24 start. */

void UiCollectionSphereStartFrameTween(UiCollectionSphere *self, u8 out)

{
  s32 i;
  GfxSprite **sprites;

  if (out == 0) {
    i = 0x21;
    do {
      sprites = (GfxSprite **)self->base.data;
      sprites[i]->flags |= 1;
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
      i++;
    } while (i < 0x22);
    UiCollectionSphereSetPageNumber(self);
    i = 0x23;
    do {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
      i++;
    } while (i < 0x25);
    i = 0x22;
    do {
      sprites = (GfxSprite **)self->base.data;
      sprites[i]->flags |= 1;
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
      i++;
    } while (i < 0x23);
  }
  else {
    i = 0x21;
    do {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
      i++;
    } while (i < 0x22);
    i = 0x23;
    do {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
      i++;
    } while (i < 0x25);
    i = 0x22;
    do {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
      i++;
    } while (i < 0x23);
  }
}
