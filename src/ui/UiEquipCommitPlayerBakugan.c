// bdc 0x0895e978 UiEquipCommitPlayerBakugan
#include "bdc.h"

/* Stores the hovered Bakugan as the current player's pick (`+0x4cdd[+0x4cdb]`) on the UiEquip
   Bakugan/gear loadout screen (task 302, `UiEquipCtor`) and increments the done-player count
   `+0x4cdc`; returns 1 if that was the last player, else advances `+0x4cdb` and returns 0. */

s32 UiEquipCommitPlayerBakugan(UiEquip *self)
{
  s8 player;
  u8 pick;

  pick = UiEquipMapBakuganIndex(self, 0, self->gridCursor);
  player = self->editPlayer;
  self->bakuganPick[player] = pick;
  self->doneCount = self->doneCount + 1;
  if (player == self->playerCount - 1) {
    return 1;
  }
  self->editPlayer = player + 1;
  return 0;
}
