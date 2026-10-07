// bdc 0x089ac2c4 UiPauseSettingsStartButtonTweens
#include "bdc.h"

/* Starts the `UiTween`s `buttonTweens` (`UiTweenBegin`, start scale 1.5) of the button-row
   sprites 0x32..0x37 for the opening (`closing` 0) or closing animation. When opening, each sprite
   is first centred, flagged 0x20, set to unit scale and made visible; 0x32..0x34 get the
   unselected texture (`UiPauseSettingsSetButtonTexture`); without the extra entry
   (`UiPauseSettingsHasExtraEntry`) sprites 0x32/0x35 shift right by 24, 0x33/0x36 by 56 and
   0x34/0x37 are hidden. */

void UiPauseSettingsStartButtonTweens(UiPauseSettings *self, u8 closing)
{
  s32 i;

  if (closing == 0) {
    for (i = 0x32; i < 0x38; i++) {
      GfxSprite *sprite;

      GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[i]);
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->flags |= 0x20;
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
      sprite = ((GfxSprite **)self->base.data)[i];
      if (i >= 0x32 && i < 0x35) {
        UiPauseSettingsSetButtonTexture(self, sprite, 0);
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      sprite->flags |= 1;
      if (UiPauseSettingsHasExtraEntry(self) == 0) {
        sprite = ((GfxSprite **)self->base.data)[i];
        switch (i - 0x32) {
        case 0:
        case 3:
          sprite->posX += 24.0f;
          sprite = ((GfxSprite **)self->base.data)[i];
          break;
        case 1:
        case 4:
          sprite->posX += 56.0f;
          sprite = ((GfxSprite **)self->base.data)[i];
          break;
        case 2:
        case 5:
          sprite->flags &= ~1u;
          sprite = ((GfxSprite **)self->base.data)[i];
          break;
        }
      } else {
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      UiTweenBegin(1.5f, closing, sprite, &self->buttonTweens[i - 0x32], 3);
    }
  } else {
    for (i = 0x32; i < 0x38; i++) {
      UiTweenBegin(1.5f, closing, ((GfxSprite **)self->base.data)[i],
                   &self->buttonTweens[i - 0x32], 3);
    }
  }
}
