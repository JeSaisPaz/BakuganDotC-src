// bdc 0x0892118c UiHologramGallerySlotPageDone
#include "bdc.h"

/* Advances the slot-list page tweens of the hologram gallery screen (task 391,
   `maybe_UiScreen391Ctor`): sprites/tweens 0x15, 0x1b, 0x18, 0x67..0x6a, 0x6c..0x6f,
   0x74..0x77, 0x70..0x73, 0x7c..0x7f, 0x5a..0x5d, 0x78..0x7b, 0x08..0x0b, 0x80 and 0x81, in that
   order (16 frames, scale 1.0, flags 1, fading out when `hide`; `UiTweenUpdate`).
   Returns true when at least one of them reports its transition finished (the count of
   finished tweens is kept in a byte). Polled by `UiHologramGalleryMainPhase`. */

bool UiHologramGallerySlotPageDone(UiHologramGallery *self, u8 hide)
{
  GfxSprite **sprites;
  u8 finished = 0;
  int i;

  for (i = 0x15; i < 0x16; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 1);
  }
  for (i = 0x1b; i < 0x1c; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 1);
  }
  for (i = 0x18; i < 0x19; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 1);
  }
  for (i = 0x67; i < 0x6b; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 1);
  }
  for (i = 0x6c; i < 0x70; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 1);
  }
  for (i = 0x74; i < 0x78; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 1);
  }
  for (i = 0x70; i < 0x74; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 1);
  }
  for (i = 0x7c; i < 0x80; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 1);
  }
  for (i = 0x5a; i < 0x5e; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 1);
  }
  for (i = 0x78; i < 0x7c; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 1);
  }
  for (i = 0x08; i < 0x0c; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 1);
  }
  for (i = 0x80; i < 0x81; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 1);
  }
  for (i = 0x81; i < 0x82; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 1);
  }
  return finished != 0;
}
