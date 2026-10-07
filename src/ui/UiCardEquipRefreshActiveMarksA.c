// bdc 0x0896a9e8 UiCardEquipRefreshActiveMarksA
#include "bdc.h"

/* Shows the group-9 mark sprite of each card slot of `UiCardEquip` whose in-use flag
   (bit 0 of `cardIds[0x10 + slot]`, i.e. `+0x29fc`) is set, with alpha 1 and its saved depth, and
   hides the others. Only the first `bakuganCount * 4` slots are touched. */

void UiCardEquipRefreshActiveMarksA(UiCardEquip *self)
{
  int i;
  int slot;
  GfxSprite *sprite;

  for (i = self->groups[9][0]; i < self->groups[9][0] + (s8)self->groups[9][1]; i++) {
    slot = i - self->groups[9][0];
    if (slot < self->bakuganCount * 4) {
      sprite = ((GfxSprite **)self->base.data)[i];
      if ((self->cardIds[0x10 + slot] & 1) != 0) {
        UiCardEquipShowSprite(self, sprite);
        ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
        ((GfxSprite **)self->base.data)[i]->posZ = self->spriteDepth[i];
      }
      else {
        sprite->flags &= ~1u;
      }
    }
  }
}
