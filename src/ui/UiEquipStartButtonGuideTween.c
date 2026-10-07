// bdc 0x08962154 UiEquipStartButtonGuideTween
#include "bdc.h"

/* Starts the appear (`out` = 0) or disappear (`out` != 0) tween (`UiTweenBegin`, start scale 1.0,
   flags 3) of the button-guide sprites (ranges `spriteIdx[0x45]`/`[0x46]` and `spriteIdx[0x47]`/`[0x48]`)
   on the UiEquip Bakugan/gear loadout screen (task 302, `UiEquipCtor`), setting sprite flag bit 0
   first. On appear, the first two sprites of the first range get button icons 2 and 1
   (`UiSetButtonIcon`). */

void UiEquipStartButtonGuideTween(UiEquip *self, u8 out)
{
  GfxSprite **sprites;
  s32 i;
  s32 d;

  for (i = self->spriteIdx[0x45]; i < self->spriteIdx[0x45] + self->spriteIdx[0x46]; i++) {
    sprites = (GfxSprite **)self->base.data;
    sprites[i]->flags |= 1;
    UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    if (out == 0) {
      d = i - self->spriteIdx[0x45];
      if (d > 0) {
        if (d < 2) {
          UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 1);
        }
      }
      else if (d >= 0) {
        UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 2);
      }
    }
  }
  for (i = self->spriteIdx[0x47]; i < self->spriteIdx[0x47] + self->spriteIdx[0x48]; i++) {
    sprites = (GfxSprite **)self->base.data;
    sprites[i]->flags |= 1;
    UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
  }
}
