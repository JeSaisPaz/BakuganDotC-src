// bdc 0x0895ea98 UiEquipStartPlayerDoneMarksTween
#include "bdc.h"

/* Starts the appear (`out` = 0) or disappear (`out` = 1) tween (`UiTweenBegin`, scale 1.5, mode
   3) of the per-player "done" marks (sprite range `spriteIdx[10]`, one per player) on the UiEquip
   Bakugan/gear loadout screen (task 302, `UiEquipCtor`), setting flag bit 0 on the marks of the
   players who already picked (`p < doneCount`) and clearing it on the others. */

void UiEquipStartPlayerDoneMarksTween(UiEquip *self, u8 out)
{
  int i;
  int p;
  GfxSprite *sprite;

  for (i = self->spriteIdx[10]; i < self->spriteIdx[10] + self->playerCount; i++) {
    p = i - self->spriteIdx[10];
    sprite = ((GfxSprite **)self->base.data)[i];
    if (p < self->playerCount) {
      if (p < self->doneCount) {
        sprite->flags |= 1;
      } else {
        sprite->flags &= ~1u;
      }
      sprite = ((GfxSprite **)self->base.data)[i];
    }
    UiTweenBegin(1.5f, out, sprite, &self->tweens[i], 3);
  }
}
