// bdc 0x0896d658 UiCardEquipEnablePointerArrows
#include "bdc.h"

/* Resets the pointer-arrow animation of `UiCardEquip` (`+0x2a70`, 0x14 bytes) and
   enables it (`+0x2a72`); when disabling also hides the arrow sprites (group 20). */

void UiCardEquipEnablePointerArrows(UiCardEquip *self, u8 enable)
{
  GfxSprite **sprites;
  int i;

  memset(self->arrowAnim, 0, 0x14);
  self->arrowsOn = enable;
  if (enable == 0) {
    for (i = self->groups[0x14][0]; i < self->groups[0x14][0] + (s8)self->groups[0x14][1]; i++) {
      sprites = (GfxSprite **)self->base.data;
      sprites[i]->flags = sprites[i]->flags & ~1u;
    }
  }
}
