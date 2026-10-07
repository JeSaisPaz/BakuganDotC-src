// bdc 0x0895ddc4 UiEquipUpdateGridTween
#include "bdc.h"

/* Advances the grid-cell tweens of the Bakugan/gear loadout screen before a battle (task 302,
   `UiEquipCtor`) started by `UiEquipStartGridTween`: the three 20-sprite blocks starting at
   spriteIdx[1], [2], [3] and the spriteIdx[0x4c] sprites starting at spriteIdx[0x4b], each through
   `UiTweenUpdate` (scale 1.5 -> 1.0 in, 1.0 -> 1.5 when `out`, over 8 frames, flags 3).
   Returns true when the u8 count of tweens that reported finished this frame is nonzero. */

bool UiEquipUpdateGridTween(UiEquip *self, u8 out)
{
  u8 done = 0;
  int block;
  int i;

  /* Three 20-sprite grid blocks; the end bound is re-read from the screen after every call. */
  for (block = 1; block <= 3; block++) {
    for (i = self->spriteIdx[block]; i < self->spriteIdx[block] + 20; i++) {
      GfxSprite *sprite = ((GfxSprite **)self->base.data)[i];
      if (out == 0) {
        done += UiTweenUpdate(1.5f, 1.0f, 8.0f, out, sprite, &self->tweens[i], 3);
      } else {
        done += UiTweenUpdate(1.0f, 1.5f, 8.0f, out, sprite, &self->tweens[i], 3);
      }
    }
  }

  /* Variable-length block: start spriteIdx[0x4b], count spriteIdx[0x4c]. */
  for (i = self->spriteIdx[0x4b]; i < self->spriteIdx[0x4b] + self->spriteIdx[0x4c]; i++) {
    GfxSprite *sprite = ((GfxSprite **)self->base.data)[i];
    if (out == 0) {
      done += UiTweenUpdate(1.5f, 1.0f, 8.0f, out, sprite, &self->tweens[i], 3);
    } else {
      done += UiTweenUpdate(1.0f, 1.5f, 8.0f, out, sprite, &self->tweens[i], 3);
    }
  }
  return done != 0;
}
