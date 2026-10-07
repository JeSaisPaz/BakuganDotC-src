// bdc 0x0895d488 UiEquipStartRandomButtonTween
#include "bdc.h"

/* Starts the appear (`out` = 0) or disappear (`out` = 1) tween (`UiTweenBegin`, scale 1.5, mode
   3) of the random-pick button below the grid (sprite range `spriteIdx[4]`, one sprite) on the UiEquip
   Bakugan/gear loadout screen (task 302, `UiEquipCtor`), making it visible. */

void UiEquipStartRandomButtonTween(UiEquip *self, u8 out)

{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  u32 i;
  u32 start = self->spriteIdx[4];

  for (i = start; i <= self->spriteIdx[4]; i++) {
    sprites[i]->flags |= 1;
    UiTweenBegin(1.5f, out, sprites[i], &self->tweens[i], 3);
  }
}
