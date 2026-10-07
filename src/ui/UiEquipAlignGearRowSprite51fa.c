// bdc 0x08962488 UiEquipAlignGearRowSprite51fa
#include "bdc.h"

/* Places the Y of sprite `+0x51fa + +0x51fc*player + entry` of the UiEquip Bakugan/gear loadout
   screen (task 302, `UiEquipCtor`) above the equipment-row sprite `+0x51aa + +0x51ac*player +
   entry`, offset by `-+0x515c` times its `scaleY`. */

void UiEquipAlignGearRowSprite51fa(UiEquip *self, u8 player, u8 entry)

{
  GfxSprite **sprites;
  GfxSprite *dst;

  sprites = (GfxSprite **)(self->base).data;
  dst = sprites[(uint)self->spriteIdx[0x4d] + (uint)self->spriteIdx[0x4e] * (uint)player + (uint)entry];
  dst->posY = sprites[(uint)self->spriteIdx[0x25] + (uint)self->spriteIdx[0x26] * (uint)player +
                      (uint)entry]->posY -
              self->rowGapB * dst->scaleY;
}
