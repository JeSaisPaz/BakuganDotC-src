// bdc 0x0895bcac UiEquipSetupSlotModels
#include "bdc.h"

/* Opens or closes the 3D pedestal area of the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`). Open (`close` = 0): creates the pedestals (`UiEquipLoadPedestalModels`), then for every
   player up to the current one (`+0x4cdb`) shows the gauge (`UiEquipShowPlayerGauge`) and loads
   the Bakugan model (`UiEquipLoadSlotModel`) of the player's pick `+0x4cdd[p]`, or of the hovered
   grid cell (`UiEquipMapBakuganIndex(screen, 0, +0x74)`) when no pick is stored yet; sets each model's alpha
   (`+0x6c`) to 1 and flag bit 1 of `+0x4ce1` (emblem spin). Close: hides every gauge, frees every
   model (`UiEquipFreeSlotModel`) and the pedestals (`UiEquipFreePedestalModels`). */

void UiEquipSetupSlotModels(UiEquip *self, bool close)

{
  int player;
  u8 id;

  if (close) {
    for (player = 0; player < self->playerCount; player++) {
      UiEquipShowPlayerGauge(self, close, (u8)player, 0);
      UiEquipFreeSlotModel(self, (u8)player);
    }
    UiEquipFreePedestalModels(self);
    return;
  }
  UiEquipLoadPedestalModels(self);
  if (self->editPlayer >= 0) {
    player = 0;
    do {
      if ((s8)self->bakuganPick[player] > 0) {
        UiEquipShowPlayerGauge(self, close, (u8)player, self->bakuganPick[player]);
        UiEquipLoadSlotModel(self, (u8)player, self->bakuganPick[player]);
      }
      else {
        id = UiEquipMapBakuganIndex(self, 0, (u8)self->gridCursor);
        UiEquipShowPlayerGauge(self, close, (u8)player, id);
        id = UiEquipMapBakuganIndex(self, 0, (u8)self->gridCursor);
        UiEquipLoadSlotModel(self, (u8)player, id);
      }
      self->bakuganModels[player]->ambient[3] = 1.0f;
      player++;
    } while (player <= self->editPlayer);
  }
  self->animFlags |= 2;
}
