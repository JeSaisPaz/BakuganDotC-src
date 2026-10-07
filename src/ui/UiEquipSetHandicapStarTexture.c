// bdc 0x0895f28c UiEquipSetHandicapStarTexture
#include "bdc.h"

/* Points `sprite` at the handicap star texture of the UiEquip Bakugan/gear loadout screen (task
   302, `UiEquipCtor`): `"c_haidi_star_2"` when `filled`, else `"c_haidi_star_1"` ("haidi" =
   handicap). */

void UiEquipSetHandicapStarTexture(UiEquip *self, GfxSprite *sprite, bool filled)

{
  char name[64];
  if (filled) {
    sprintf(name,"c_haidi_star_2");
  }
  else {
    sprintf(name,"c_haidi_star_1");
  }
  sprite->texture = GfxFindTexture(name);
  return;
}

