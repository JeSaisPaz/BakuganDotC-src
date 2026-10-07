// bdc 0x089938d4 UiUnlockCodeRegisterCode
#include "bdc.h"

/* Stores an accepted 10-digit code of `UiUnlockCode` in the profile: takes the
   first free slot of the 8-bit mask `unlockCodeSlots` (slot 8 when all are taken), fetches its
   reward entry (`UiUnlockCodeGetRewardEntry` → `reward`), marks the slot used and saves the 10
   normalized digits in `unlockCodes[slot]`. */

void UiUnlockCodeRegisterCode(UiUnlockCode *self)
{
  u16 entry[3];
  u8 slot;
  int i;

  slot = 0;
  for (i = 0; i < 8; i++) {
    if ((SaveGetProfile()->data->unlockCodeSlots & (1 << i)) == 0) {
      break;
    }
    slot++;
  }
  UiUnlockCodeGetRewardEntry(entry, self, slot);
  self->reward[0] = entry[0];
  self->reward[1] = entry[1];
  self->reward[2] = entry[2];
  SaveGetProfile()->data->unlockCodeSlots |= (u8)(1 << slot);
  for (i = 0; i < 10; i++) {
    SaveGetProfile()->data->unlockCodes[slot][i] = (u16)self->normalized[i];
  }
}
