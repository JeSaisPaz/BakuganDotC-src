// bdc 0x089620c0 UiEquipAlignGearRowSprite51de
#include "bdc.h"

/* Places the Y of sprite `+0x51de + +0x51e0*player + entry` of the UiEquip Bakugan/gear loadout
   screen (task 302, `UiEquipCtor`) below the equipment-row sprite `+0x51aa + +0x51ac*player +
   entry`, offset by `gearRowOfs[player][entry]` times its `scaleY`. */

void UiEquipAlignGearRowSprite51de(UiEquip *self, u8 player, u8 entry)
{
  GfxSprite **sprites;
  GfxSprite *anchor;
  GfxSprite *dst;

  sprites = (GfxSprite **)(self->base).data;
  anchor = sprites[self->spriteIdx[0x25] + self->spriteIdx[0x26] * player + entry];
  dst = sprites[self->spriteIdx[0x3f] + self->spriteIdx[0x40] * player + entry];
  dst->posY = anchor->posY + self->gearRowOfs[player][entry] * dst->scaleY;
}
