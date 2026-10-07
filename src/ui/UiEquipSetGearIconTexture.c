// bdc 0x0896207c UiEquipSetGearIconTexture
#include "bdc.h"

/* Points `sprite` at the equipment icon `"c_setting_soubi_l_sol_%02d"` (`gearId` + 1) on the
   UiEquip Bakugan/gear loadout screen (task 302, `UiEquipCtor`). */

void UiEquipSetGearIconTexture(UiEquip *self, GfxSprite *sprite, u8 gearId)

{
  char name[64];

  sprintf(name,"c_setting_soubi_l_sol_%02d",gearId + 1);
  sprite->texture = GfxFindTexture(name);
  return;
}

