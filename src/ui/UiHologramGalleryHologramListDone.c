// bdc 0x08921938 UiHologramGalleryHologramListDone
#include "bdc.h"

/* Advances the hologram list page tweens of the hologram gallery screen (task 391,
   `maybe_UiScreen391Ctor`, class prefix `UiHologramGallery`; `fix_*` sprites, `DWHologramHelp`
   texts; main update `UiHologramGalleryMainPhase` with step `+0x2c`) (`UiTweenUpdate`): tween i drives
   sprite i of `base.data`, ranges 0x15, 0x1b, 0x18, 0x39-0x3a, 0x49-0x4a, 0x3d-0x3e, 0x66, 0x5e-0x61,
   0x59, 0x4d-0x50, 0x85 with flags 1 and 0x86-0x8b, 0x8d-0x92, 0x96-0x9b with flags 5. Returns true when
   the u8 count of finished tweens is non-zero (i.e. at least one finished, modulo 256 wrap). */

bool UiHologramGalleryHologramListDone(UiHologramGallery *self, u8 hide)
{
  u8 finished = 0;
  int i;

  for (i = 0x15; i < 0x16; i++)
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 1);
  for (i = 0x1b; i < 0x1c; i++)
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 1);
  for (i = 0x18; i < 0x19; i++)
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 1);
  for (i = 0x39; i < 0x3b; i++)
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 1);
  for (i = 0x49; i < 0x4b; i++)
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 1);
  for (i = 0x3d; i < 0x3f; i++)
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 1);
  for (i = 0x66; i < 0x67; i++)
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 1);
  for (i = 0x5e; i < 0x62; i++)
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 1);
  for (i = 0x59; i < 0x5a; i++)
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 1);
  for (i = 0x4d; i < 0x51; i++)
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 1);
  for (i = 0x85; i < 0x86; i++)
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 1);
  for (i = 0x86; i < 0x8c; i++)
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 5);
  for (i = 0x8d; i < 0x93; i++)
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 5);
  for (i = 0x96; i < 0x9c; i++)
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 5);
  return finished != 0;
}
