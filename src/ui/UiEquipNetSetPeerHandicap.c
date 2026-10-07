// bdc 0x08962598 UiEquipNetSetPeerHandicap
#include "bdc.h"

/* Applies a remote player's handicap on the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`): if it differs from `+0x520c` of the record (the remote-player records at
   `+0x5200 + player*0x28` (`+0` Bakugan, `+4/+8` equipment, `+0xc` handicap, `+0x10/+0x14` extra
   words, `+0x18` Bakugan-changed flag, `+0x24` equipment-changed counter)), stores it there and in
   `+0x5020[player]` and redraws the stars (`UiEquipRefreshHandicapStars`). Ignores `player`
   outside 0..3. */

void UiEquipNetSetPeerHandicap(UiEquip *self, u32 player, s32 handicap)

{
  if (((-1 < (int)player) && ((int)player < 4)) &&
     (self->peer[player].handicap != handicap)) {
    self->peer[player].handicap = handicap;
    self->handicap[player] = (u8)handicap;
    UiEquipRefreshHandicapStars(self,(u8)player);
  }
  return;
}

