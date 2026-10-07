// bdc 0x0895f2e8 UiEquipAlignSpritesToAnchor
#include "bdc.h"

/* Positions player `player`'s `count` sprites starting at `base + player*count` relative to the
   anchor sprite `anchorBase + player*anchorStride` of the UiEquip Bakugan/gear loadout screen (task
   302, `UiEquipCtor`): X/Y = anchor position + the per-sprite offset pair stored at `+0x4118 +
   idx*8` times the anchor's current scale. */

void UiEquipAlignSpritesToAnchor(UiEquip *self, u16 anchorBase, u16 anchorStride, u16 base, u16 count, u8 player)
{
  int i;
  u16 anchorIdx = (u16)(anchorBase + player * anchorStride);

  for (i = 0; i < count; i++) {
    GfxSprite **sprites = (GfxSprite **)self->base.data;
    u16 idx = (u16)(base + player * count + i);
    GfxSprite *anchor = sprites[anchorIdx];
    sprites[idx]->posX = anchor->posX + self->spritePos[idx][0] * anchor->scaleX;
    sprites = (GfxSprite **)self->base.data;
    anchor = sprites[anchorIdx];
    sprites[idx]->posY = anchor->posY + self->spritePos[idx][1] * anchor->scaleY;
  }
}
