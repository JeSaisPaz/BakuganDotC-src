// bdc 0x089130b0 UiUpgradeTweenBasePanels
#include "bdc.h"

/* Starts the show/hide tweens of the base panels of the Bakugan upgrade screen: sprites/tweens
   0..4 scale from 1.5 (`UiTweenBegin`, flags 3), sprite 10 slides on X by 128 px (flags 7),
   sprite 11 slides on Y by 64 px and sprite 12 on Y by -64 px (flags 0xb), all with
   `UiTweenBeginSlide`. Showing (`hide == 0`) first sets bit 0 of each sprite's `flags` and
   slides from the offset to 0; hiding slides from 0 to the offset and fades out. */

void UiUpgradeTweenBasePanels(UiUpgrade *self, u8 hide)
{
  int i;

  if (hide == 0) {
    for (i = 0; i < 5; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      UiTweenBegin(1.5f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 10; i < 11; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      UiTweenBeginSlide(1.5f, 128.0f, 0.0f, hide, ((GfxSprite **)self->base.data)[i],
                        &self->tweens[i], 7);
    }
    for (i = 11; i < 12; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      UiTweenBeginSlide(1.5f, 64.0f, 0.0f, hide, ((GfxSprite **)self->base.data)[i],
                        &self->tweens[i], 0xb);
    }
    for (i = 12; i < 13; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      UiTweenBeginSlide(1.5f, -64.0f, 0.0f, hide, ((GfxSprite **)self->base.data)[i],
                        &self->tweens[i], 0xb);
    }
  }
  else {
    for (i = 0; i < 5; i++) {
      UiTweenBegin(1.5f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 10; i < 11; i++) {
      UiTweenBeginSlide(1.5f, 0.0f, 128.0f, hide, ((GfxSprite **)self->base.data)[i],
                        &self->tweens[i], 7);
    }
    for (i = 11; i < 12; i++) {
      UiTweenBeginSlide(1.5f, 0.0f, 64.0f, hide, ((GfxSprite **)self->base.data)[i],
                        &self->tweens[i], 0xb);
    }
    for (i = 12; i < 13; i++) {
      UiTweenBeginSlide(1.5f, 0.0f, -64.0f, hide, ((GfxSprite **)self->base.data)[i],
                        &self->tweens[i], 0xb);
    }
  }
}
