// bdc 0x0895d208 UiEquipResetIntroCounter
#include "bdc.h"

/* Clears the intro frame counter `introFrames` of the UiEquip Bakugan/gear loadout screen
   (task 302, `UiEquipCtor`). */

void UiEquipResetIntroCounter(UiEquip *self)
{
    self->introFrames = 0;
}
