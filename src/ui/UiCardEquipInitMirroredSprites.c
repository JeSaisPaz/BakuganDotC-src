// bdc 0x0896c51c UiCardEquipInitMirroredSprites
#include "bdc.h"

/* Sets the mirrored halves of `UiCardEquip`: flips the tab highlight vertically
   (`GfxSpriteFlipV`; group-3 sprite 2 with fewer than 3 Bakugan, else sprite 3), mirrors
   (`GfxSpriteFlipU`) the right-hand triple of each of the first `bakuganCount` 6-sprite gauge
   arrow sets (group 14), and of the 2 (fewer than 3 Bakugan) or 4 pointer arrows (group 20)
   mirrors the odd ones and flips the ones from index 2 on vertically. */

void UiCardEquipInitMirroredSprites(UiCardEquip *self)
{
  GfxSprite **sprites;
  int arrowCount;
  int i;

  sprites = (GfxSprite **)self->base.data;
  if (self->bakuganCount < 3) {
    GfxSpriteFlipV(sprites[self->groups[3][0] + 2]);
    arrowCount = 2;
  }
  else {
    GfxSpriteFlipV(sprites[self->groups[3][0] + 3]);
    arrowCount = 4;
  }
  for (i = self->groups[14][0]; i < self->groups[14][0] + (s8)self->groups[14][1]; i++) {
    if (i - self->groups[14][0] < self->bakuganCount * 6 &&
        !((i - self->groups[14][0]) % 6 < 3)) {
      GfxSpriteFlipU(((GfxSprite **)self->base.data)[i]);
    }
  }
  for (i = self->groups[20][0]; i < self->groups[20][0] + arrowCount; i++) {
    if (((i - self->groups[20][0]) & 1) != 0) {
      GfxSpriteFlipU(((GfxSprite **)self->base.data)[i]);
    }
    if (!(i - self->groups[20][0] < 2)) {
      GfxSpriteFlipV(((GfxSprite **)self->base.data)[i]);
    }
  }
}
