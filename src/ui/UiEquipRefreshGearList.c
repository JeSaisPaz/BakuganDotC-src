// bdc 0x08960df8 UiEquipRefreshGearList
#include "bdc.h"

/* Refreshes player `player`'s equipment panel on the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`). Four sprite ranges `spriteIdx[a] + spriteIdx[a+1]*player` are each shown with
   `UiEquipShowPanelSprite`: ranges 0x27 and 0x2d are hidden again where the entry is not chosen
   (bit 0 of `panelCardFlags[player*4 + k]` clear); range 0x39 shows the thumbnail of
   `gearPick[player][k]` (`UiCardSetThumbnailTexture`) or is hidden when it is 0xff; range 0x3f gets
   button icon 2 (`UiSetButtonIcon`) and is hidden like the first two. */

void UiEquipRefreshGearList(UiEquip *self, u8 player)
{
  int i;
  u8 gear;
  GfxSprite *sprite;

  for (i = self->spriteIdx[0x27] + self->spriteIdx[0x28] * player;
       i < self->spriteIdx[0x27] + self->spriteIdx[0x28] * (player + 1); i++) {
    UiEquipShowPanelSprite(self, (u16)i);
    if ((self->panelCardFlags[player * 4 +
                              (i - (self->spriteIdx[0x27] + self->spriteIdx[0x28] * player))] & 1) == 0) {
      ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
    }
  }
  for (i = self->spriteIdx[0x2d] + self->spriteIdx[0x2e] * player;
       i < self->spriteIdx[0x2d] + self->spriteIdx[0x2e] * (player + 1); i++) {
    UiEquipShowPanelSprite(self, (u16)i);
    if ((self->panelCardFlags[player * 4 +
                              (i - (self->spriteIdx[0x2d] + self->spriteIdx[0x2e] * player))] & 1) == 0) {
      ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
    }
  }
  for (i = self->spriteIdx[0x39] + self->spriteIdx[0x3a] * player;
       i < self->spriteIdx[0x39] + self->spriteIdx[0x3a] * (player + 1); i++) {
    UiEquipShowPanelSprite(self, (u16)i);
    sprite = ((GfxSprite **)self->base.data)[i];
    gear = self->gearPick[player][i - (self->spriteIdx[0x39] + self->spriteIdx[0x3a] * player)];
    if (gear == 0xff) {
      sprite->flags &= ~1u;
    } else {
      UiCardSetThumbnailTexture(sprite, gear);
    }
  }
  for (i = self->spriteIdx[0x3f] + self->spriteIdx[0x40] * player;
       i < self->spriteIdx[0x3f] + self->spriteIdx[0x40] * (player + 1); i++) {
    UiEquipShowPanelSprite(self, (u16)i);
    UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 2);
    if ((self->panelCardFlags[player * 4 +
                              (i - (self->spriteIdx[0x3f] + self->spriteIdx[0x40] * player))] & 1) == 0) {
      ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
    }
  }
}
