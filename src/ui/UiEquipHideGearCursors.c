// bdc 0x089603fc UiEquipHideGearCursors
#include "bdc.h"

/* Hides player `player`'s equipment-panel cursors on the UiEquip Bakugan/gear loadout screen (task
   302, `UiEquipCtor`): the list cursor `+0x51be + player`, the OK cursor `+0x51a6 + player` and
   the extra highlight copy sprite (`data[+0x4fb4]`). */

void UiEquipHideGearCursors(UiEquip *self, u8 player)

{
  GfxSprite **sprites;

  sprites = (GfxSprite **)(self->base).data;
  sprites[(uint)self->spriteIdx[0x2f] + (uint)player]->flags &= ~1u;
  sprites[(uint)self->spriteIdx[0x23] + (uint)player]->flags &= ~1u;
  sprites[self->spriteCount]->flags &= ~1u;
}
