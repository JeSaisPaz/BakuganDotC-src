// bdc 0x0892e610 UiBakuganSelectStartArrows
#include "bdc.h"

/* Arms the arrow record `arrowsOn..` (4 bytes, zeroed) of the Bakugan select screen
   (`UiBakuganSelectCtor`, task 371) with `on`. When off, hides the arrow sprites 0x72..0x7d.
   When on, each sprite `0x72 + i` belongs to entry `i / 2`: if that entry's Bakugan bit is set in
   `unselectableMask[1]` the sprite is shown (layer mask 2, alpha 1, blend mode 2) at the entry's
   position `spritePos[26 + i / 2]` plus `layerOffset[1 + i % 2]`, otherwise it is hidden. */

void UiBakuganSelectStartArrows(UiBakuganSelect *self, u8 on)

{
  GfxSprite **sprites;
  s32 i;
  u8 entry;

  memset(&self->arrowsOn, 0, 4);
  self->arrowsOn = on;
  if (self->arrowsOn) {
    for (i = 0; i < 12; i++) {
      entry = (u8)(i / 2);
      sprites = (GfxSprite **)self->base.data;
      if ((self->unselectableMask[1] & (1u << (self->entries[entry].bakugan & 0x1f))) != 0) {
        sprites[0x72 + i]->flags |= 1;
        sprites[0x72 + i]->layerMask = 2;
        sprites[0x72 + i]->alpha = 1.0f;
        sprites[0x72 + i]->posX = self->spritePos[entry + 26][0] + self->layerOffset[1 + i % 2][0];
        sprites[0x72 + i]->posY = self->spritePos[entry + 26][1] + self->layerOffset[1 + i % 2][1];
        sprites[0x72 + i]->blendMode = 2;
      } else {
        sprites[0x72 + i]->flags &= ~1u;
      }
    }
  } else {
    for (i = 0x72; i < 0x7e; i++) {
      ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
    }
  }
}
