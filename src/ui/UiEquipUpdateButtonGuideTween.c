// bdc 0x0896230c UiEquipUpdateButtonGuideTween
#include "bdc.h"

/* Advances the tween started by `UiEquipStartButtonGuideTween` of the button-guide sprites
   (sprite ranges `spriteIdx[0x45]`/count `spriteIdx[0x46]` and `spriteIdx[0x47]`/count
   `spriteIdx[0x48]`) on the UiEquip Bakugan/gear loadout screen (task 302, `UiEquipCtor`) with
   `UiTweenUpdate` (scale 1.0→1.0, 8 frames, flags 1); returns true when at least one sprite
   tween reported finished (the per-sprite results are summed in a u8). */

bool UiEquipUpdateButtonGuideTween(UiEquip *self, u8 out)
{
  u8 done;
  int i;

  done = 0;
  for (i = self->spriteIdx[0x45]; i < self->spriteIdx[0x45] + self->spriteIdx[0x46]; i++) {
    done += UiTweenUpdate(1.0f, 1.0f, 8.0f, out, ((GfxSprite **)self->base.data)[i],
                          &self->tweens[i], 1);
  }
  for (i = self->spriteIdx[0x47]; i < self->spriteIdx[0x47] + self->spriteIdx[0x48]; i++) {
    done += UiTweenUpdate(1.0f, 1.0f, 8.0f, out, ((GfxSprite **)self->base.data)[i],
                          &self->tweens[i], 1);
  }
  return done != 0;
}
