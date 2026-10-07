// bdc 0x0898e7f8 UiCollectionFigureStartDetailPanelTween
#include "bdc.h"

/* Starts the tween of the detail panel sprites (sprites 41, 42, 44 and 45 of `base.data`, tweens
   of the same index) of the figure collection screen (task 314, `maybe_UiScreen314Ctor`) when
   an entry is opened (`out == 0`) or closed. Opening: each sprite gets the visible flag and layer
   mask 4, sprite 41 is also reset to grey tint 0.5 and alpha 0, then
   `UiTweenBegin``(1.0, 0, sprite, tween, 3)`. Closing: only
   `UiTweenBegin``(1.0, out, sprite, tween, 3)` for each. */

void UiCollectionFigureStartDetailPanelTween(UiCollectionFigure *self, u8 out)
{
  GfxSprite *sprite;
  int i;

  if (out == 0) {
    for (i = 41; i < 43; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 4;
      sprite = ((GfxSprite **)self->base.data)[i];
      if (i == 41) {
        sprite->tint[0] = 0.5f;
        sprite->tint[1] = 0.5f;
        sprite->tint[2] = 0.5f;
        sprite->alpha = 0.0f;
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      UiTweenBegin(1.0f, 0, sprite, &self->tweens[i], 3);
    }
    for (i = 44; i < 46; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 4;
      UiTweenBegin(1.0f, 0, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  } else {
    for (i = 41; i < 43; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 44; i < 46; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
}
