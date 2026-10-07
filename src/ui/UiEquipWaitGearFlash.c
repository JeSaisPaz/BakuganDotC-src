// bdc 0x08960cbc UiEquipWaitGearFlash
#include "bdc.h"

/* Steps the flash slots started by `UiEquipFlashGearSelection` on the UiEquip Bakugan/gear
   loadout screen (task 302, `UiEquipCtor`) (`UiFlashStep` slots 0 and 1 on the list row, slot 0
   on the OK row); returns 1 when the last one is done. */

s32 UiEquipWaitGearFlash(UiEquip *self)
{
    u8 row = self->activeRow;

    if (SaveGetProfileFlag0() != 0) {
        row = self->gearRowFlags[*(s32 *)self->localPlayer];
    }
    if (row == 0) {
        UiFlashStep(0);
        if (UiFlashStep(1) != 0) {
            return 1;
        }
    } else {
        if (UiFlashStep(0) != 0) {
            return 1;
        }
    }
    return 0;
}
