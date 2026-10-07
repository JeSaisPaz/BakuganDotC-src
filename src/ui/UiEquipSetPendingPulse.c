// bdc 0x0895ee54 UiEquipSetPendingPulse
#include "bdc.h"

/* Enables or disables the pulse of the waiting players' labels (`+0x4f84`) on the UiEquip
   Bakugan/gear loadout screen (task 302, `UiEquipCtor`). */

void UiEquipSetPendingPulse(UiEquip *self, bool enable)

{
  self->pulseOn = enable;
  return;
}

