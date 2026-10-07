// bdc 0x08992238 UiUnlockCodeSyncRewards
#include "bdc.h"

/* For each of the 8 special rewards (`UiUnlockCodeGetRewardEntry`) whose kind (low byte) is 0,
   sets bit `i` of the profile's `unlockCodeSlots` mask (save block `+0x604`) when the matching bit
   (index = entry halfword `+2`) is already set in the profile bitset `newItemGroups` (`+0x5d3`);
   returns how many of the 8 mask bits are set (rewards already claimed). */

s32 UiUnlockCodeSyncRewards(UiUnlockCode *self)

{
  SaveProfileData *data;
  s32 i;
  s32 idx;
  u8 count;
  u16 entry[3];

  for (i = 0; i < 8; i++) {
    UiUnlockCodeGetRewardEntry(entry, self, (u8)i);
    idx = entry[1];
    if ((u8)entry[0] == 0) {
      if ((u8)(SaveGetProfile()->data->newItemGroups[idx / 8] & (1 << (idx % 8))) != 0) {
        data = SaveGetProfile()->data;
        data->unlockCodeSlots |= 1 << i;
      }
    }
  }
  count = 0;
  for (i = 0; i < 8; i++) {
    if ((u8)(SaveGetProfile()->data->unlockCodeSlots & (1 << i)) != 0) {
      count++;
    }
  }
  return count;
}
