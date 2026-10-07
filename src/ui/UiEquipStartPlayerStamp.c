// bdc 0x0895e17c UiEquipStartPlayerStamp
#include "bdc.h"

/* (Re)starts player `player`'s stamp record (12 bytes at `+0x4f34 + player*0xc`) on the UiEquip
   Bakugan/gear loadout screen (task 302, `UiEquipCtor`) for `bakuganId`: unless the record is
   idle and `bakuganId` is 0, clears it, stores the id at `+2`, sets the active flag `+0` and, for a
   non-zero id, the attribute cell `+3` (`UiBakuganGetAttribute`, Bakugan id → attribute table
   `0x08ac13c8`). */

void UiEquipStartPlayerStamp(UiEquip *self, u8 player, u8 bakuganId)

{
  u8 *rec;

  rec = self->stampRec[player];
  if (rec[2] != 0 || bakuganId != 0) {
    memset(rec, 0, 0xc);
    rec[2] = bakuganId;
    rec[0] = 1;
    if (rec[2] != 0) {
      rec[3] = UiBakuganGetAttribute(bakuganId);
    }
  }
}
