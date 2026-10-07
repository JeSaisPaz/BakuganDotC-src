// bdc 0x089202cc UiHologramGalleryCountPanelDone
#include "bdc.h"

/* Advances the count-panel tweens of sprites 0x1e..0x32 (tween records `tweens[0x1e..0x32]`,
   16 frames, flags 9, fading out when `hide`) started by `UiHologramGalleryTweenCountPanel`.
   Returns true when at least one of them reports its transition finished (`UiTweenUpdate`
   returns true once done; the count of finished tweens is kept in a byte). */

bool UiHologramGalleryCountPanelDone(UiHologramGallery *self, u8 hide)
{
  GfxSprite **sprites;
  u8 finished = 0;
  int i;

  for (i = 0x1e; i < 0x25; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 9);
  }
  for (i = 0x25; i < 0x2c; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 9);
  }
  for (i = 0x2c; i < 0x33; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 9);
  }
  return finished != 0;
}
