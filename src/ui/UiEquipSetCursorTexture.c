// bdc 0x0895ada8 UiEquipSetCursorTexture
#include "bdc.h"

/* Points `sprite` at player `player`'s grid cursor texture `"cha_ico_cursol_%02d"` (player + 1) on
   the UiEquip Bakugan/gear loadout screen (task 302, `UiEquipCtor`). */

void UiEquipSetCursorTexture(UiEquip *self, GfxSprite *sprite, u8 player)

{
  char name[64];

  sprintf(name,"cha_ico_cursol_%02d",player + 1);
  sprite->texture = GfxFindTexture(name);
  return;
}

