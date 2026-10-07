// bdc 0x0895e540 UiEquipPreviewHoveredBakugan
#include "bdc.h"

/* Previews the Bakugan under the grid cursor for the current player (`+0x4cdb`) on the UiEquip
   Bakugan/gear loadout screen (task 302, `UiEquipCtor`): hides the player's gauge
   (`UiEquipShowPlayerGauge`); on the random button or a Bakugan whose bit in the profile bit field
   `bakuganBitsA` (`+0x5be`) is clear it clears the stamp; otherwise shows the gauge, loads the model
   (`UiEquipLoadSlotModel`, alpha 1) and starts the stamp (`UiEquipStartPlayerStamp`). */

void UiEquipPreviewHoveredBakugan(UiEquip *self)

{
  SaveProfile *profile;
  u8 player;
  int id;
  u8 bit;

  player = (u8)self->editPlayer;
  UiEquipShowPlayerGauge(self, true, player, UiEquipMapBakuganIndex(self, 0, (u8)self->gridCursor));
  if (self->onRandom != 0) {
    UiEquipStartPlayerStamp(self, (u8)self->editPlayer, 0);
    return;
  }
  profile = SaveGetProfile();
  id = UiEquipMapBakuganIndex(self, 0, (u8)self->gridCursor);
  player = (u8)self->editPlayer;
  bit = (u8)(profile->data->bakuganBitsA[id / 8] & (1 << (id % 8)));
  if (bit == 0) {
    UiEquipStartPlayerStamp(self, player, 0);
    return;
  }
  UiEquipShowPlayerGauge(self, false, player, UiEquipMapBakuganIndex(self, 0, (u8)self->gridCursor));
  player = (u8)self->editPlayer;
  UiEquipLoadSlotModel(self, player, UiEquipMapBakuganIndex(self, 0, (u8)self->gridCursor));
  self->bakuganModels[self->editPlayer]->ambient[3] = 1.0f;
  player = (u8)self->editPlayer;
  UiEquipStartPlayerStamp(self, player, UiEquipMapBakuganIndex(self, 0, (u8)self->gridCursor));
}
