// bdc 0x08959460 UiEquipInitSlotSprites
#include "bdc.h"

/* Prepares the per-player slot sprites of the `UiEquip` loadout screen. For three
   anchor groups (`spriteIdx[0x19]` stride `[0x1a]`, `[0x11]` stride `[0x12]`, `[0x17]` stride
   `[0x18]`) it centres each player's anchor sprites, enables linear filtering, resets their
   scale/rotation and records their positions in `spritePos`, then lays out the dependent ranges
   relative to those anchors via `UiEquipInitSlotOffsets`. Between the second and third group
   it stores, per player, the vertical gaps between the sprites of ranges `[0x3f]` and `[0x25]` in
   `gearRowOfs`. */

void UiEquipInitSlotSprites(UiEquip *self)
{
  int player;
  int i;
  u16 slot;
  u16 a;
  u16 b;

  /* Group 1: anchors spriteIdx[0x19] + player * spriteIdx[0x1a]. */
  for (player = 0; player < self->playerCount; player++) {
    for (i = 0; i < (int)self->spriteIdx[0x1a]; i++) {
      slot = (u16)(self->spriteIdx[0x19] + player * self->spriteIdx[0x1a] + i);
      GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[slot]);
      ((GfxSprite **)self->base.data)[slot]->flags |= 0x20;
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[slot], 1.0f, 1.0f, 0.0f);
      self->spritePos[slot][0] = ((GfxSprite **)self->base.data)[slot]->posX;
      self->spritePos[slot][1] = ((GfxSprite **)self->base.data)[slot]->posY;
    }
  }
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x19], self->spriteIdx[0x1a], self->spriteIdx[0x1b],
                         self->spriteIdx[0x1c]);
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x19], self->spriteIdx[0x1a], self->spriteIdx[0x1d],
                         self->spriteIdx[0x1e]);
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x19], self->spriteIdx[0x1a], self->spriteIdx[0x41],
                         self->spriteIdx[0x42]);
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x19], self->spriteIdx[0x1a], self->spriteIdx[0x49],
                         self->spriteIdx[0x4a]);

  /* Group 2: anchors spriteIdx[0x11] + player * spriteIdx[0x12], spriteIdx[0x1a] per player. */
  for (player = 0; player < self->playerCount; player++) {
    for (i = 0; i < (int)self->spriteIdx[0x1a]; i++) {
      slot = (u16)(self->spriteIdx[0x11] + player * self->spriteIdx[0x12] + i);
      GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[slot]);
      ((GfxSprite **)self->base.data)[slot]->flags |= 0x20;
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[slot], 1.0f, 1.0f, 0.0f);
      self->spritePos[slot][0] = ((GfxSprite **)self->base.data)[slot]->posX;
      self->spritePos[slot][1] = ((GfxSprite **)self->base.data)[slot]->posY;
    }
  }
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x11], self->spriteIdx[0x12], self->spriteIdx[0x13],
                         self->spriteIdx[0x14]);
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x11], self->spriteIdx[0x12], self->spriteIdx[0x15],
                         self->spriteIdx[0x16]);
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x11], self->spriteIdx[0x12], self->spriteIdx[0x1f],
                         self->spriteIdx[0x20]);
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x11], self->spriteIdx[0x12], self->spriteIdx[0x21],
                         self->spriteIdx[0x22]);
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x11], self->spriteIdx[0x12], self->spriteIdx[0x23],
                         self->spriteIdx[0x24]);
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x11], self->spriteIdx[0x12], self->spriteIdx[0x25],
                         self->spriteIdx[0x26]);
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x11], self->spriteIdx[0x12], self->spriteIdx[0x27],
                         self->spriteIdx[0x28]);
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x11], self->spriteIdx[0x12], self->spriteIdx[0x29],
                         self->spriteIdx[0x2a]);
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x11], self->spriteIdx[0x12], self->spriteIdx[0x2d],
                         self->spriteIdx[0x2e]);
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x11], self->spriteIdx[0x12], self->spriteIdx[0x2f],
                         self->spriteIdx[0x30]);
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x11], self->spriteIdx[0x12], self->spriteIdx[0x31],
                         self->spriteIdx[0x32]);
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x11], self->spriteIdx[0x12], self->spriteIdx[0x33],
                         self->spriteIdx[0x34]);
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x11], self->spriteIdx[0x12], self->spriteIdx[0x37],
                         self->spriteIdx[0x38]);
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x11], self->spriteIdx[0x12], self->spriteIdx[0x39],
                         self->spriteIdx[0x3a]);
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x11], self->spriteIdx[0x12], self->spriteIdx[0x3d],
                         self->spriteIdx[0x3e]);
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x11], self->spriteIdx[0x12], self->spriteIdx[0x3f],
                         self->spriteIdx[0x40]);
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x11], self->spriteIdx[0x12], self->spriteIdx[0x43],
                         self->spriteIdx[0x44]);
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x11], self->spriteIdx[0x12], self->spriteIdx[0x4d],
                         self->spriteIdx[0x4e]);

  /* Per-player vertical gap between range [0x3f] and range [0x25] sprites. */
  for (player = 0; player < self->playerCount; player++) {
    for (i = 0; i < (int)self->spriteIdx[0x40]; i++) {
      a = (u16)(self->spriteIdx[0x3f] + player * self->spriteIdx[0x40] + i);
      b = (u16)(self->spriteIdx[0x25] + player * self->spriteIdx[0x26] + i);
      self->gearRowOfs[player][i] =
          ((GfxSprite **)self->base.data)[a]->posY - ((GfxSprite **)self->base.data)[b]->posY;
    }
  }

  /* Group 3: anchors spriteIdx[0x17] + player * spriteIdx[0x18]. */
  for (player = 0; player < self->playerCount; player++) {
    for (i = 0; i < (int)self->spriteIdx[0x18]; i++) {
      slot = (u16)(self->spriteIdx[0x17] + player * self->spriteIdx[0x18] + i);
      GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[slot]);
      ((GfxSprite **)self->base.data)[slot]->flags |= 0x20;
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[slot], 1.0f, 1.0f, 0.0f);
      self->spritePos[slot][0] = ((GfxSprite **)self->base.data)[slot]->posX;
      self->spritePos[slot][1] = ((GfxSprite **)self->base.data)[slot]->posY;
    }
  }
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x17], self->spriteIdx[0x18], self->spriteIdx[0x2b],
                         self->spriteIdx[0x2c]);
  UiEquipInitSlotOffsets(self, self->spriteIdx[0x17], self->spriteIdx[0x18], self->spriteIdx[0x3b],
                         self->spriteIdx[0x3c]);
}
