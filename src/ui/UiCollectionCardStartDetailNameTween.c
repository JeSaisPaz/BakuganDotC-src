// bdc 0x089851c8 UiCollectionCardStartDetailNameTween
#include "bdc.h"

/* Starts the tween of the card name sprite (39, label via `UiCollectionCardSetCardLabel`, layer
   4) and the related detail sprite 50 of `UiCollectionCard` when a card is
   opened (`out` = 0) or closed. Opening also relabels sprite 39 with the selected slot's card,
   makes both sprites visible (flags bit 0) and moves them to layer 4 before the tween starts. */

void UiCollectionCardStartDetailNameTween(UiCollectionCard *self, u8 out)
{
  int i;

  if (out == 0) {
    for (i = 39; i < 40; i++) {
      UiCollectionCardSetCardLabel(self, ((GfxSprite **)self->base.data)[i],
                                   self->slots[self->cursor + self->page * 4]);
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 4;
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 50; i < 51; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 4;
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  } else {
    for (i = 39; i < 40; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 50; i < 51; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
}
