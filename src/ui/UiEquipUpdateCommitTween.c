// bdc 0x08961364 UiEquipUpdateCommitTween
#include "bdc.h"

/* Advances the fade-out started by `UiEquipStartCommitTween` of player `player`'s equipment rows
   on the UiEquip Bakugan/gear loadout screen (task 302, `UiEquipCtor`) (ranges `+0x518e/+0x5190`,
   `+0x51b6/+0x51b8`, `+0x51d6/+0x51d8`, `UiEquipUpdateSpriteFadeTween`) and keeps the rows
   `+0x51b6` and `+0x51d6` attached to the panel `+0x518e` (`UiEquipAlignSpritesToAnchor`);
   returns true when any fade reports done (the u8 sum of the tween results is non-zero). */

bool UiEquipUpdateCommitTween(UiEquip *self, u8 player)
{
  int i;
  u8 busy = 0;

  for (i = self->spriteIdx[0x17] + self->spriteIdx[0x18] * player;
       i < self->spriteIdx[0x17] + self->spriteIdx[0x18] * (player + 1); i++) {
    busy += UiEquipUpdateSpriteFadeTween(self, 0, (u16)i);
  }
  for (i = self->spriteIdx[0x2b] + self->spriteIdx[0x2c] * player;
       i < self->spriteIdx[0x2b] + self->spriteIdx[0x2c] * (player + 1); i++) {
    busy += UiEquipUpdateSpriteFadeTween(self, 0, (u16)i);
  }
  UiEquipAlignSpritesToAnchor(self, self->spriteIdx[0x17], self->spriteIdx[0x18],
                              self->spriteIdx[0x2b], self->spriteIdx[0x2c], player);
  for (i = self->spriteIdx[0x3b] + self->spriteIdx[0x3c] * player;
       i < self->spriteIdx[0x3b] + self->spriteIdx[0x3c] * (player + 1); i++) {
    busy += UiEquipUpdateSpriteFadeTween(self, 0, (u16)i);
  }
  UiEquipAlignSpritesToAnchor(self, self->spriteIdx[0x17], self->spriteIdx[0x18],
                              self->spriteIdx[0x3b], self->spriteIdx[0x3c], player);
  return busy != 0;
}
