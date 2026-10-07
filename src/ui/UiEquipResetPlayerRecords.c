// bdc 0x089572cc UiEquipResetPlayerRecords
#include "bdc.h"

/* Clears the four 0x28-byte player records at `+0x5200` of `UiEquip` (gauge value
   `+0x520c` = 100); unless `keepModels`, also creates the pedestal models
   (`UiEquipLoadPedestalModels`) and sets update bits 2|4 in `+0x4ce1`. */

void UiEquipResetPlayerRecords(UiEquip *self, u8 keepModels)
{
  int i;

  for (i = 0; i < 4; i++) {
    memset(self->remote[i], 0, 0x28);
    self->peer[i].bakugan = 0;
    self->peer[i].handicap = 100;
    self->peer[i].extra10 = 0;
  }
  if (keepModels == 0) {
    UiEquipLoadPedestalModels(self);
    self->animFlags = self->animFlags | 6;
  }
}
