// bdc 0x08959de0 UiEquipGetPlayerSpriteBase
#include "bdc.h"

/* Returns the first sprite index of player `player`'s 7-sprite group in `UiEquip`
   (`+0x5178` + 7·player). */

s32 UiEquipGetPlayerSpriteBase(UiEquip *self, u8 player)

{
  return (uint)self->spriteIdx[0xc] + (uint)player * 7;
}

