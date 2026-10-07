// bdc 0x0895d8b4 UiEquipSetFaceTexture
#include "bdc.h"

/* Points `sprite` at the Bakugan face icon `"baku_face_%02d"` for `bakuganId` (`id - 1`, 0 also
   maps to face 00) on the UiEquip Bakugan/gear loadout screen (task 302, `UiEquipCtor`). */

void UiEquipSetFaceTexture(UiEquip *self, GfxSprite *sprite, u8 bakuganId)
{
  char name[64];

  if (bakuganId == 0) {
    sprintf(name, "baku_face_%02d", 0);
  } else {
    sprintf(name, "baku_face_%02d", bakuganId - 1);
  }
  sprite->texture = GfxFindTexture(name);
}
