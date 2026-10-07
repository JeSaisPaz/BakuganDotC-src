// bdc 0x0895ebb8 UiEquipUpdatePlayerDoneMarksTween
#include "bdc.h"

/* Advances the tween started by `UiEquipStartPlayerDoneMarksTween` of the per-player "done"
   marks (sprite range `spriteIdx[10]`, one per player) on the UiEquip Bakugan/gear loadout
   screen (task 302, `UiEquipCtor`) (`UiTweenUpdate`, 1.5→1.0 in, 1.0→1.5 out, 8 frames,
   flags 3); returns true once any of the tweens has finished (they run in lockstep), false
   while all are running or when there are no players. */

bool UiEquipUpdatePlayerDoneMarksTween(UiEquip *self, u8 out)
{
  int i;
  u8 doneCount = 0;

  for (i = self->spriteIdx[10]; i < self->spriteIdx[10] + self->playerCount; i++) {
    GfxSprite *sprite = ((GfxSprite **)self->base.data)[i];
    if (out == 0) {
      doneCount += UiTweenUpdate(1.5f, 1.0f, 8.0f, 0, sprite, &self->tweens[i], 3);
    } else {
      doneCount += UiTweenUpdate(1.0f, 1.5f, 8.0f, out, sprite, &self->tweens[i], 3);
    }
  }
  return doneCount != 0;
}
