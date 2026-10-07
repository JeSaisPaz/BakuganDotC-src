// bdc 0x08963764 UiEquipStartGearPanelTween
#include "bdc.h"

/* Opens (`out` = 0, after filling the rows with `UiEquipBuildGearChoices`) or closes the
   equipment panel of player `player` on the Bakugan/gear loadout screen before a battle (task 302,
   `UiEquipCtor`): for each of its sprite ranges (`spriteIdx[a] + spriteIdx[b] * player` up to
   the next player's start) starts the fade tween through `UiEquipStartSpriteFadeTween`, and
   sets the per-range textures/visibility: OK buttons reset (`UiEquipSetOkButtonTexture`,
   inactive), the large card images of `panelCardIds` (`UiGauntletSetupSetCardTexture`; hidden
   when 0xff), the thumbnails of `gearPick` (`UiCardSetThumbnailTexture`; hidden when 0xff),
   button icons 0 and 2 (`UiSetButtonIcon`), and hides the sprites of entries whose
   `panelCardFlags` bit0 is clear, of empty entries, and the marker of entries whose equipment
   bit is set in the profile's `newItemGroups[0x15..]`. */

void UiEquipStartGearPanelTween(UiEquip *self, bool out, u8 player)
{
  int next = player + 1;
  int i;
  GfxSprite *sprite;
  int card;

  if (!out) {
    UiEquipBuildGearChoices(self, player);
  }
  for (i = self->spriteIdx[0x11] + self->spriteIdx[0x12] * player;
       i < self->spriteIdx[0x11] + self->spriteIdx[0x12] * next; i++) {
    UiEquipStartSpriteFadeTween(self, out, (u16)i);
  }
  for (i = self->spriteIdx[0x15] + self->spriteIdx[0x16] * player;
       i < self->spriteIdx[0x15] + self->spriteIdx[0x16] * next; i++) {
    UiEquipStartSpriteFadeTween(self, out, (u16)i);
  }
  for (i = self->spriteIdx[0x1f] + self->spriteIdx[0x20] * player;
       i < self->spriteIdx[0x1f] + self->spriteIdx[0x20] * next; i++) {
    UiEquipSetOkButtonTexture(self, ((GfxSprite **)self->base.data)[i], false);
    UiEquipStartSpriteFadeTween(self, out, (u16)i);
  }
  for (i = self->spriteIdx[0x21] + self->spriteIdx[0x22] * player;
       i < self->spriteIdx[0x21] + self->spriteIdx[0x22] * next; i++) {
    UiEquipStartSpriteFadeTween(self, out, (u16)i);
  }
  for (i = self->spriteIdx[0x25] + self->spriteIdx[0x26] * player;
       i < self->spriteIdx[0x25] + self->spriteIdx[0x26] * next; i++) {
    UiEquipStartSpriteFadeTween(self, out, (u16)i);
  }
  for (i = self->spriteIdx[0x27] + self->spriteIdx[0x28] * player;
       i < self->spriteIdx[0x27] + self->spriteIdx[0x28] * next; i++) {
    UiEquipStartSpriteFadeTween(self, out, (u16)i);
    if ((self->panelCardFlags[player * 4 + i - (self->spriteIdx[0x27] + self->spriteIdx[0x28] * player)] & 1) == 0) {
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->flags &= ~1u;
    }
  }
  for (i = self->spriteIdx[0x29] + self->spriteIdx[0x2a] * player;
       i < self->spriteIdx[0x29] + self->spriteIdx[0x2a] * next; i++) {
    UiEquipStartSpriteFadeTween(self, out, (u16)i);
  }
  for (i = self->spriteIdx[0x2d] + self->spriteIdx[0x2e] * player;
       i < self->spriteIdx[0x2d] + self->spriteIdx[0x2e] * next; i++) {
    UiEquipStartSpriteFadeTween(self, out, (u16)i);
    if ((self->panelCardFlags[player * 4 + i - (self->spriteIdx[0x2d] + self->spriteIdx[0x2e] * player)] & 1) == 0) {
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->flags &= ~1u;
    }
  }
  for (i = self->spriteIdx[0x31] + self->spriteIdx[0x32] * player;
       i < self->spriteIdx[0x31] + self->spriteIdx[0x32] * next; i++) {
    UiEquipStartSpriteFadeTween(self, out, (u16)i);
  }
  for (i = self->spriteIdx[0x33] + self->spriteIdx[0x34] * player;
       i < self->spriteIdx[0x33] + self->spriteIdx[0x34] * next; i++) {
    UiEquipStartSpriteFadeTween(self, out, (u16)i);
  }
  for (i = self->spriteIdx[0x37] + self->spriteIdx[0x38] * player;
       i < self->spriteIdx[0x37] + self->spriteIdx[0x38] * next; i++) {
    UiEquipStartSpriteFadeTween(self, out, (u16)i);
    sprite = ((GfxSprite **)self->base.data)[i];
    card = self->panelCardIds[player * 4 + i - (self->spriteIdx[0x37] + self->spriteIdx[0x38] * player)];
    if (card == 0xff) {
      sprite->flags &= ~1u;
    } else {
      UiGauntletSetupSetCardTexture(sprite, (u8)card);
    }
  }
  for (i = self->spriteIdx[0x39] + self->spriteIdx[0x3a] * player;
       i < self->spriteIdx[0x39] + self->spriteIdx[0x3a] * next; i++) {
    UiEquipStartSpriteFadeTween(self, out, (u16)i);
    sprite = ((GfxSprite **)self->base.data)[i];
    card = self->gearPick[player][i - (self->spriteIdx[0x39] + self->spriteIdx[0x3a] * player)];
    if (card == 0xff) {
      sprite->flags &= ~1u;
    } else {
      UiCardSetThumbnailTexture(sprite, (u32)card);
    }
  }
  for (i = self->spriteIdx[0x3d] + self->spriteIdx[0x3e] * player;
       i < self->spriteIdx[0x3d] + self->spriteIdx[0x3e] * next; i++) {
    UiEquipStartSpriteFadeTween(self, out, (u16)i);
    UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 0);
  }
  for (i = self->spriteIdx[0x3f] + self->spriteIdx[0x40] * player;
       i < self->spriteIdx[0x3f] + self->spriteIdx[0x40] * next; i++) {
    UiEquipStartSpriteFadeTween(self, out, (u16)i);
    UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 2);
    if ((self->panelCardFlags[player * 4 + i - (self->spriteIdx[0x3f] + self->spriteIdx[0x40] * player)] & 1) == 0) {
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->flags &= ~1u;
    }
  }
  for (i = self->spriteIdx[0x43] + self->spriteIdx[0x44] * player;
       i < self->spriteIdx[0x43] + self->spriteIdx[0x44] * next; i++) {
    UiEquipStartSpriteFadeTween(self, out, (u16)i);
    if (self->panelCardIds[player * 4 + i - (self->spriteIdx[0x43] + self->spriteIdx[0x44] * player)] != 0xff) {
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->flags &= ~1u;
    }
  }
  for (i = self->spriteIdx[0x4d] + self->spriteIdx[0x4e] * player;
       i < self->spriteIdx[0x4d] + self->spriteIdx[0x4e] * next; i++) {
    UiEquipStartSpriteFadeTween(self, out, (u16)i);
    if (self->panelCardIds[player * 4 + i - (self->spriteIdx[0x4d] + self->spriteIdx[0x4e] * player)] == 0xff) {
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->flags &= ~1u;
    } else {
      SaveProfile *profile = SaveGetProfile();
      card = self->panelCardIds[player * 4 + i - (self->spriteIdx[0x4d] + self->spriteIdx[0x4e] * player)];
      if ((u8)(profile->data->newItemGroups[0x15 + card / 8] & (1 << (card % 8))) != 0) {
        sprite = ((GfxSprite **)self->base.data)[i];
        sprite->flags &= ~1u;
      }
    }
  }
}
