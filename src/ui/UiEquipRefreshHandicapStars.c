// bdc 0x0895f968 UiEquipRefreshHandicapStars
#include "bdc.h"

/* Redraws player `player`'s handicap display on the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`) from its handicap value `+0x5020[player]` (50, 100 or 150): the star sprites
   (`+0x519a + player*+0x519c`, star i filled when value ≥ 50*(i+1),
   `UiEquipSetHandicapStarTexture`) and the cell of the sprites `+0x51f2 + player*+0x51f4`
   (`GfxSpriteSetCell`, row value/50*(i+1) - 1). */

void UiEquipRefreshHandicapStars(UiEquip *self, u8 player)
{
  int i;
  int k;

  for (i = self->spriteIdx[0x1d] + self->spriteIdx[0x1e] * player;
       i < self->spriteIdx[0x1d] + self->spriteIdx[0x1e] * (player + 1); i++) {
    k = i - self->spriteIdx[0x1d];
    UiEquipSetHandicapStarTexture(
        self, ((GfxSprite **)self->base.data)[i],
        self->handicap[k / self->spriteIdx[0x1e]] / ((k % self->spriteIdx[0x1e]) * 50 + 50) != 0);
  }
  for (i = self->spriteIdx[0x49] + self->spriteIdx[0x4a] * player;
       i < self->spriteIdx[0x49] + self->spriteIdx[0x4a] * (player + 1); i++) {
    k = i - self->spriteIdx[0x49];
    GfxSpriteSetCell(
        ((GfxSprite **)self->base.data)[i], 0.0f,
        (float)(u8)(self->handicap[k / self->spriteIdx[0x4a]] / ((k % self->spriteIdx[0x4a]) * 50 + 50) - 1));
  }
}
