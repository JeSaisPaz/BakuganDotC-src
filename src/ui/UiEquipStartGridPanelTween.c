// bdc 0x0895d25c UiEquipStartGridPanelTween
#include "bdc.h"

/* Starts the appear (`out` = 0) or disappear (`out` = 1) tween (`UiTweenBegin`, scale 1.5, mode
   3) of the grid panel sprite (sprite range `spriteIdx[0]`, one sprite) on the UiEquip Bakugan/gear
   loadout screen (task 302, `UiEquipCtor`), making it visible. */

void UiEquipStartGridPanelTween(UiEquip *self, u8 out)

{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  u32 i;
  u32 start = self->spriteIdx[0];

  for (i = start; i <= start; i++) {
    sprites[i]->flags |= 1;
    UiTweenBegin(1.5f, out, sprites[i], &self->tweens[i], 3);
  }
}
