// bdc 0x0896268c UiEquipNetReceivePeerRecord
#include "bdc.h"

/* Applies one received selection record of remote player `player` on the UiEquip Bakugan/gear
   loadout screen (task 302, `UiEquipCtor`): `record` = {Bakugan, equipment 0, equipment 1,
   handicap, word4, word5}; stores words 4/5 at `+0x5210/+0x5214` and forwards the rest to
   `UiEquipNetSetPeerBakugan`, `UiEquipNetSetPeerHandicap` and `UiEquipNetSetPeerGear`.
   Ignores `player` outside 0..3. */

void UiEquipNetReceivePeerRecord(UiEquip *self, s32 player, const u32 *record)

{
  u32 extra14;
  u32 bakuganId;
  u32 handicap;
  u32 gear1;
  u32 gear0;
  
  bakuganId = *record;
  gear0 = record[1];
  gear1 = record[2];
  handicap = record[3];
  extra14 = record[5];
  if ((-1 < player) && (player < 4)) {
    self->peer[player].extra10 = record[4];
    self->peer[player].extra14 = extra14;
    UiEquipNetSetPeerBakugan(self,player,bakuganId);
    UiEquipNetSetPeerHandicap(self,player,handicap);
    UiEquipNetSetPeerGear(self,player,gear0,gear1);
  }
  return;
}

