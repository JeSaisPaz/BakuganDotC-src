// bdc 0x089601d0 UiEquipSetOkButtonTexture
#include "bdc.h"

/* Points `sprite` at the OK button texture of the equipment panel of the UiEquip Bakugan/gear
   loadout screen (task 302, `UiEquipCtor`): `"c_set_OK_bo_1"` when `active`, else
   `"c_set_OK_bo_2"`. */

void UiEquipSetOkButtonTexture(UiEquip *self, GfxSprite *sprite, bool active)

{
  char name[64];
  if (active) {
    sprintf(name,"c_set_OK_bo_1");
  }
  else {
    sprintf(name,"c_set_OK_bo_2");
  }
  sprite->texture = GfxFindTexture(name);
  return;
}

