// bdc 0x0895adec UiEquipSetRandomCursorTexture
#include "bdc.h"

/* Points `sprite` at player `player`'s second cursor texture `"cha_ico_cursol_%02d_2"` (player +
   1), used when the cursor sits on the random-pick button below the grid (`+0x75` set) of the
   UiEquip Bakugan/gear loadout screen (task 302, `UiEquipCtor`). */

void UiEquipSetRandomCursorTexture(UiEquip *self, GfxSprite *sprite, u8 player)

{
  char name[64];

  sprintf(name,"cha_ico_cursol_%02d_2",player + 1);
  sprite->texture = GfxFindTexture(name);
  return;
}

