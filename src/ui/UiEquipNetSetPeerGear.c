// bdc 0x089625fc UiEquipNetSetPeerGear
#include "bdc.h"

/* Applies a remote player's two chosen equipment entries on the UiEquip Bakugan/gear loadout screen
   (task 302, `UiEquipCtor`): updates the equipment words `gear[0..1]` of the remote-player record
   `peer[player]` and the chosen pair `gearPick[player]`; when anything changed sets the
   equipment-changed counter `gearChanged` to 3.
   Ignores `player` outside 0..3. */

void UiEquipNetSetPeerGear(UiEquip *self, s32 player, u32 gear0, u32 gear1)
{
  bool changed;
  u8 pick0;
  u8 pick1;

  if ((-1 < player) && (player < 4)) {
    changed = false;
    if (self->peer[player].gear[0] != gear0) {
      self->peer[player].gear[0] = gear0;
      changed = true;
    }
    if (self->peer[player].gear[1] != gear1) {
      self->peer[player].gear[1] = gear1;
      changed = true;
    }
    pick0 = self->gearPick[player][0];
    if (pick0 != gear0) {
      self->gearPick[player][0] = (u8)gear0;
      changed = true;
    }
    pick1 = self->gearPick[player][1];
    if (pick1 != gear1) {
      self->gearPick[player][1] = (u8)gear1;
      changed = true;
    }
    if (changed) {
      self->peer[player].gearChanged = 3;
    }
  }
}
