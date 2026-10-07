// bdc 0x0896a888 UiCardEquipRefreshCardMarkers
#include "bdc.h"

/* Refreshes the group-16 marker sprite of each card slot of `UiCardEquip` (only the
   first `bakuganCount * 4` slots): makes it visible (`UiCardEquipShowSprite`), then sets alpha 1
   when the slot's in-use flag (bit 0 of `cardIds[0x10 + slot]`, i.e. `+0x29fc`) is set or hides it
   otherwise, and restores its saved depth in both cases. */

void UiCardEquipRefreshCardMarkers(UiCardEquip *self)
{
  int i;
  int slot;
  GfxSprite *sprite;

  for (i = self->groups[16][0]; i < self->groups[16][0] + (s8)self->groups[16][1]; i++) {
    if (i - self->groups[16][0] < self->bakuganCount * 4) {
      UiCardEquipShowSprite(self, ((GfxSprite **)self->base.data)[i]);
      slot = i - self->groups[16][0];
      sprite = ((GfxSprite **)self->base.data)[i];
      if ((self->cardIds[0x10 + (slot / 4) * 4 + slot % 4] & 1) != 0) {
        sprite->alpha = 1.0f;
      }
      else {
        sprite->flags &= ~1u;
      }
      ((GfxSprite **)self->base.data)[i]->posZ = self->spriteDepth[i];
    }
  }
}
