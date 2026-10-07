// bdc 0x0898f1e8 UiCollectionFigureStartMotionButtonsTween
#include "bdc.h"

/* Starts the appear (`out` 0) or disappear tween of the motion-button sprites 0x38..0x3a and
   0x3c..0x3e of `UiCollectionFigure` (figure collection screen, task 314).
   On appear the sprites are shown on layer 8, sprites 0x39/0x3a get button icons 4/5 and sprites
   0x3c.. are reset to white tint and zero alpha first. */

void UiCollectionFigureStartMotionButtonsTween(UiCollectionFigure *self, u8 out)

{
  int i;
  int j;
  GfxSprite *sprite;

  if (out == 0) {
    for (i = 0x38; i < 0x3b; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      sprite = ((GfxSprite **)self->base.data)[i];
      j = i - 0x38;
      if (j < 2) {
        if (j > 0) {
          UiSetButtonIcon(sprite, 4);
          sprite = ((GfxSprite **)self->base.data)[i];
        }
      }
      else if (j < 3) {
        UiSetButtonIcon(sprite, 5);
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      sprite->layerMask = 8;
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0x3c; i < 0x3f; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 8;
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->tint[0] = 1.0f;
      sprite->tint[1] = 1.0f;
      sprite->tint[2] = 1.0f;
      sprite->alpha = 0.0f;
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
  else {
    for (i = 0x38; i < 0x3b; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0x3c; i < 0x3f; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
  return;
}
