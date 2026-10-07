// bdc 0x0895d68c UiEquipStartSprite516aTween
#include "bdc.h"

/* Starts the appear (`out` = 0) or disappear (`out` = 1) tween (`UiTweenBegin`, scale 1.5, mode
   3) of a single background sprite (sprite range ``+0x516a`, one sprite`) on the UiEquip
   Bakugan/gear loadout screen (task 302, `UiEquipCtor`), making it visible and placing it at z =
   -15. */

void UiEquipStartSprite516aTween(UiEquip *self, u8 out)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  s32 i;

  for (i = self->spriteIdx[5]; i < self->spriteIdx[5] + 1; i++) {
    sprites[i]->flags |= 1;
    sprites[i]->posZ = -15.0f;
    UiTweenBegin(1.5f, out, sprites[i], &self->tweens[i], 3);
  }
}
