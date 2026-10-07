// bdc 0x0895cd44 UiEquipStartPlayerIconsTween
#include "bdc.h"

/* Starts the appear (`out` = 0) or disappear (`out` = 1) tween (`UiTweenBegin`, scale 1.5, mode
   3) of the per-player icon sprites (sprite range `spriteIdx[9]`, one per player) on the UiEquip
   Bakugan/gear loadout screen (task 302, `UiEquipCtor`), setting each one's flag bit 0 and its
   sheet cell to column 0, row `playerIconCell[p]` (`GfxSpriteSetCell`). */

void UiEquipStartPlayerIconsTween(UiEquip *self, u8 out)
{
  int i;
  GfxSprite *sprite;

  for (i = self->spriteIdx[9]; i < self->spriteIdx[9] + self->playerCount; i++) {
    sprite = ((GfxSprite **)self->base.data)[i];
    if (i - self->spriteIdx[9] < self->playerCount) {
      sprite->flags |= 1;
      sprite = ((GfxSprite **)self->base.data)[i];
    }
    UiTweenBegin(1.5f, out, sprite, &self->tweens[i], 3);
    GfxSpriteSetCell(((GfxSprite **)self->base.data)[i], 0.0f,
                     (float)self->playerIconCell[i - self->spriteIdx[9]]);
  }
}
