// bdc 0x0898eb2c UiCollectionFigureStartDetailNameTween
#include "bdc.h"

/* Starts the tween of the two name sprites of the detail view (sprites 43 and 54 of
   `base.data`, tweens 43 and 54) of the figure collection screen (task 314,
   `maybe_UiScreen314Ctor`). Fading in (`out == 0`): first sets sprite 43 to the name of the
   selected entry (`entryIds[page * 6 + cursor]`, `UiCollectionFigureSetNameTexture`), and for
   both sprites sets the visible flag and layer mask 4 before `UiTweenBegin``(1.0, 0, sprite,
   tween, 3)`. Fading out: only `UiTweenBegin``(1.0, out, sprite, tween, 3)` for both. The
   original code runs each sprite as a one-iteration loop. */

void UiCollectionFigureStartDetailNameTween(UiCollectionFigure *self, u8 out)
{
  GfxSprite **sprites;

  if (out == 0) {
    sprites = (GfxSprite **)self->base.data;
    UiCollectionFigureSetNameTexture(self, sprites[43],
                                     self->entryIds[self->cursor + self->page * 6]);
    sprites = (GfxSprite **)self->base.data;
    sprites[43]->flags |= 1;
    sprites = (GfxSprite **)self->base.data;
    sprites[43]->layerMask = 4;
    sprites = (GfxSprite **)self->base.data;
    UiTweenBegin(1.0f, 0, sprites[43], &self->tweens[43], 3);

    sprites = (GfxSprite **)self->base.data;
    sprites[54]->flags |= 1;
    sprites = (GfxSprite **)self->base.data;
    sprites[54]->layerMask = 4;
    sprites = (GfxSprite **)self->base.data;
    UiTweenBegin(1.0f, 0, sprites[54], &self->tweens[54], 3);
  } else {
    sprites = (GfxSprite **)self->base.data;
    UiTweenBegin(1.0f, out, sprites[43], &self->tweens[43], 3);
    sprites = (GfxSprite **)self->base.data;
    UiTweenBegin(1.0f, out, sprites[54], &self->tweens[54], 3);
  }
}
