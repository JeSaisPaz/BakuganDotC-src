// bdc 0x089188ec UiAdvSelectTweenFrame
#include "bdc.h"

/* Starts the frame tweens of the adventure partner-select screen with `UiTweenBeginSlide`
   (start scale 1.5): sprite 0 with tween 0 at `+0x78` (flags 7) and sprite 2 with tween 2 at
   `+0xc8` (flags 0xb). Showing (`hide == 0`) marks both sprites visible (flags bit 0) and slides
   them from 64 to 0; hiding slides from 0 to 64 with fade-out and only re-marks sprite 2 visible.
   Completion is polled by `UiAdvSelectFrameDone`. */

void UiAdvSelectTweenFrame(UiAdvSelect *self, u8 hide)
{
  int i;

  if (hide == 0) {
    for (i = 0; i < 1; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      UiTweenBeginSlide(1.5f, 64.0f, 0.0f, hide, ((GfxSprite **)self->base.data)[i],
                        &self->tweens[i], 7);
    }
    for (i = 2; i < 3; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      UiTweenBeginSlide(1.5f, 64.0f, 0.0f, hide, ((GfxSprite **)self->base.data)[i],
                        &self->tweens[i], 0xb);
    }
  }
  else {
    for (i = 0; i < 1; i++) {
      UiTweenBeginSlide(1.5f, 0.0f, 64.0f, hide, ((GfxSprite **)self->base.data)[i],
                        &self->tweens[i], 7);
    }
    for (i = 2; i < 3; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      UiTweenBeginSlide(1.5f, 0.0f, 64.0f, hide, ((GfxSprite **)self->base.data)[i],
                        &self->tweens[i], 0xb);
    }
  }
}
