// bdc 0x0895d210 UiEquipStepIntroCounter
#include "bdc.h"

/* Increments the intro frame counter `+0x4d04` of the UiEquip Bakugan/gear loadout screen (task
   302, `UiEquipCtor`); on the 4th frame opens the pedestal area (`UiEquipSetupSlotModels` with
   `close` = 0). */

void UiEquipStepIntroCounter(UiEquip *self)
{
    self->introFrames = self->introFrames + 1;
    if ((float)self->introFrames == 4.0f) {
        UiEquipSetupSlotModels(self, false);
    }
}
