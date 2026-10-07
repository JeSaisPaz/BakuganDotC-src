// bdc 0x0896ab44 UiCardEquipRefreshEmptySlots
#include "bdc.h"

/* Shows the empty-slot placeholder sprites (group 17) of `UiCardEquip`: for
   each sprite of the group whose slot index (sprite - first) is below `bakuganCount * 4` and whose
   card id `cardIds[slot]` is 0xff (empty), makes it visible (`UiCardEquipShowSprite`), sets its
   alpha to 1 and restores its saved depth `spriteDepth[sprite]`. */

void UiCardEquipRefreshEmptySlots(UiCardEquip *self)
{
  u32 first;
  s32 i;
  s32 slot;

  first = self->groups[17][0];
  for (i = first; i < (s32)(first + (s8)self->groups[17][1]); i++) {
    slot = i - first;
    if (slot < self->bakuganCount * 4 &&
        self->cardIds[(slot / 4) * 4 + slot % 4] == 0xff) {
      UiCardEquipShowSprite(self, ((GfxSprite **)self->base.data)[i]);
      ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
      ((GfxSprite **)self->base.data)[i]->posZ = self->spriteDepth[i];
      first = self->groups[17][0];
    }
  }
}
