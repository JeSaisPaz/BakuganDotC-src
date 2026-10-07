// bdc 0x0897cd6c UiCollectionSphereStartDetailNameTween
#include "bdc.h"

/* Starts the appear (`out` 0) or disappear tween of the detail-view name sprite 45 and label
   sprite 56 of `UiCollectionSphere` (tweens 45/56, start scale 1, flags 3).
   When appearing it first sets the name texture: categories 0/1 use
   `UiCollectionSphereSetNameTexture` with `entryIds[page * 6 + cursor]`; category 2 uses
   `UiCollectionSphereSetSpecialNameTexture` with, by `UiCollectionSphereGetPageKind`,
   `entryIds[(page / 3) * 6 + cursor]` (kind 0 or 0xff), `kind1Ids[page / 3]` (kind 1) or
   `kind2Ids[page / 3]` (kind 2); other categories keep the texture. Both sprites are then shown on
   layer mask 4. */

void UiCollectionSphereStartDetailNameTween(UiCollectionSphere *self, u8 out)

{
  GfxSprite **sprites;
  s8 category;

  if (out == 0) {
    category = self->category;
    if (category < 2) {
      if (category >= 0) {
        UiCollectionSphereSetNameTexture(self, ((GfxSprite **)self->base.data)[45],
                                         self->entryIds[self->cursor + self->page * 6]);
      }
    } else if (category < 3) {
      switch (UiCollectionSphereGetPageKind(self, (u8)self->page)) {
      case 0:
      case 0xff:
        UiCollectionSphereSetSpecialNameTexture(
            self, ((GfxSprite **)self->base.data)[45],
            self->entryIds[self->cursor + (self->page / 3) * 6]);
        break;
      case 1:
        UiCollectionSphereSetSpecialNameTexture(self, ((GfxSprite **)self->base.data)[45],
                                                self->kind1Ids[self->page / 3]);
        break;
      case 2:
        UiCollectionSphereSetSpecialNameTexture(self, ((GfxSprite **)self->base.data)[45],
                                                self->kind2Ids[self->page / 3]);
        break;
      default:
        break;
      }
    }
    sprites = (GfxSprite **)self->base.data;
    sprites[45]->flags |= 1;
    sprites[45]->layerMask = 4;
    UiTweenBegin(1.0f, out, sprites[45], &self->tweens[45], 3);

    sprites = (GfxSprite **)self->base.data;
    sprites[56]->flags |= 1;
    sprites[56]->layerMask = 4;
    UiTweenBegin(1.0f, out, sprites[56], &self->tweens[56], 3);
  } else {
    UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[45], &self->tweens[45], 3);
    UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[56], &self->tweens[56], 3);
  }
  return;
}
