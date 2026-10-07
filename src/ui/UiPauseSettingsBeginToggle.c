// bdc 0x089adb84 UiPauseSettingsBeginToggle
#include "bdc.h"

/* Starts the slide tween of the toggle-value sprite of `UiPauseSettings`
   (sprite 61, `data+0xf4`, `toggleTween`). Opening: sets its cell from profile `adviceOff`
   (`UiPauseSettingsSetToggleCell`), makes it visible and slides it in from +32 px X with alpha
   (`UiTweenBeginSlide` flags 5); when the toggle is unavailable (`UiPauseSettingsAdviceAvailable` == 0) it is
   tinted 50 % grey with alpha 0. Closing: centres the pivot, resets scale and slides it out by 32 px (flags 7,
   alpha + scale + X). The single sprite is handled by a one-iteration loop, kept as in the asm. */

void UiPauseSettingsBeginToggle(UiPauseSettings *self, u8 closing)
{
  s32 i;
  UiTween *tween;
  GfxSprite *sprite;

  if (closing == 0) {
    tween = &self->toggleTween;
    for (i = 0x3d; i < 0x3e; i++, tween++) {
      sprite = ((GfxSprite **)self->base.data)[i];
      UiPauseSettingsSetToggleCell(self, sprite, SaveGetProfile()->data->adviceOff);
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->flags |= 1;
      UiTweenBeginSlide(1.0f, 32.0f, 0.0f, 0, ((GfxSprite **)self->base.data)[i], tween, 5);
      if (UiPauseSettingsAdviceAvailable(self) == 0) {
        sprite = ((GfxSprite **)self->base.data)[i];
        sprite->tint[0] = 0.5f;
        sprite->tint[1] = 0.5f;
        sprite->tint[2] = 0.5f;
        sprite->alpha = 0.0f;
      }
    }
  } else {
    tween = &self->toggleTween;
    for (i = 0x3d; i < 0x3e; i++, tween++) {
      GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[i]);
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->flags |= 0x20;
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
      UiTweenBeginSlide(1.0f, 0.0f, 32.0f, 1, ((GfxSprite **)self->base.data)[i], tween, 7);
    }
  }
}
