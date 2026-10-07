// bdc 0x0895e92c UiEquipWaitGridFlash
#include "bdc.h"

/* Steps the flash slots started by `UiEquipFlashGridSelection` on the UiEquip Bakugan/gear
   loadout screen (task 302, `UiEquipCtor`) (`UiFlashStep`: slot 1 only on the grid, then slot
   0); returns 1 when slot 0 is done. */

s32 UiEquipWaitGridFlash(UiEquip *self)
{
    if (self->onRandom == 0) {
        UiFlashStep(1);
    }
    if (UiFlashStep(0) != 0) {
        return 1;
    }
    return 0;
}
