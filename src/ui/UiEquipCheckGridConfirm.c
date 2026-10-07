// bdc 0x0895e700 UiEquipCheckGridConfirm
#include "bdc.h"

/* Checks the confirm button (pressed bit 0x4000, Cross) on the Bakugan grid of the UiEquip
   Bakugan/gear loadout screen (task 302, `UiEquipCtor`): returns 0 when not pressed, 2 when the
   hovered Bakugan is not owned (profile bit field `bakuganBitsA`), else 1 (also on the random button). */

s32 UiEquipCheckGridConfirm(UiEquip *self)
{
  SaveProfile *profile;
  s32 bit;

  if ((self->base.pad->pressed & 0x4000) == 0) {
    return 0;
  }
  if (self->onRandom == 0) {
    profile = SaveGetProfile();
    bit = UiEquipMapBakuganIndex(self, 0, (u8)self->gridCursor);
    if ((u8)(profile->data->bakuganBitsA[bit / 8] & (1 << (bit % 8))) == 0) {
      return 2;
    }
  }
  return 1;
}
