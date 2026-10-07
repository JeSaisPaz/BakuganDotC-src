// bdc 0x0895fb38 UiEquipShowPlayerSlot
#include "bdc.h"

/* Shows (`show` != 0: sprite flag bit 0 set) or hides (bit 0 cleared) player `player`'s slot sprites
   on the UiEquip Bakugan/gear loadout screen (task 302, `UiEquipCtor`): the five sprite ranges
   `spriteIdx[a] + spriteIdx[a+1]*player` for a = 0x19, 0x1b, 0x1d, 0x41, 0x49. While showing range
   0x1d (handicap stars) it calls `UiEquipRefreshHandicapStars` once per sprite of that range. */

void UiEquipShowPlayerSlot(UiEquip *self, bool show, u8 player)
{
  int i;
  GfxSprite *sprite;

  for (i = self->spriteIdx[0x19] + self->spriteIdx[0x1a] * player;
       i < self->spriteIdx[0x19] + self->spriteIdx[0x1a] * (player + 1); i++) {
    sprite = ((GfxSprite **)self->base.data)[i];
    if (show) {
      sprite->flags |= 1;
    } else {
      sprite->flags &= ~1u;
    }
  }
  for (i = self->spriteIdx[0x1b] + self->spriteIdx[0x1c] * player;
       i < self->spriteIdx[0x1b] + self->spriteIdx[0x1c] * (player + 1); i++) {
    sprite = ((GfxSprite **)self->base.data)[i];
    if (show) {
      sprite->flags |= 1;
    } else {
      sprite->flags &= ~1u;
    }
  }
  for (i = self->spriteIdx[0x1d] + self->spriteIdx[0x1e] * player;
       i < self->spriteIdx[0x1d] + self->spriteIdx[0x1e] * (player + 1); i++) {
    sprite = ((GfxSprite **)self->base.data)[i];
    if (show) {
      sprite->flags |= 1;
      UiEquipRefreshHandicapStars(self, player);
    } else {
      sprite->flags &= ~1u;
    }
  }
  for (i = self->spriteIdx[0x41] + self->spriteIdx[0x42] * player;
       i < self->spriteIdx[0x41] + self->spriteIdx[0x42] * (player + 1); i++) {
    sprite = ((GfxSprite **)self->base.data)[i];
    if (show) {
      sprite->flags |= 1;
    } else {
      sprite->flags &= ~1u;
    }
  }
  for (i = self->spriteIdx[0x49] + self->spriteIdx[0x4a] * player;
       i < self->spriteIdx[0x49] + self->spriteIdx[0x4a] * (player + 1); i++) {
    sprite = ((GfxSprite **)self->base.data)[i];
    if (show) {
      sprite->flags |= 1;
    } else {
      sprite->flags &= ~1u;
    }
  }
}
