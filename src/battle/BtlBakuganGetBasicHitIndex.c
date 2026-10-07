// bdc 0x08886fe8 BtlBakuganGetBasicHitIndex
#include "bdc.h"

/* Maps the unit's current melee combo to a row of its basic-hit table (0..0x15). The combo step
   `comboStep` is clamped to 0..3. Combo `comboIndex` 0 gives the step, with `comboAirVariant` set
   steps 2/3 become 4/5; combo 1 gives the same + 6; combo 2 step + 0xc; combo 3 step + 0x10;
   combos 4/5 give 0x14; combos 6/7 give 0x15 when `attackId` is in 6..0x15, else 0x14. Returns -1
   for a NULL unit or any other combo. */
s32 BtlBakuganGetBasicHitIndex(BtlBakugan *self, s32 attackId)
{
    s32 step;

    if (self == NULL) {
        return -1;
    }
    step = self->comboStep;
    if (step < 0) {
        step = 0;
    } else if (step > 3) {
        step = 3;
    }
    switch (self->comboIndex) {
    case 0:
        if (self->comboAirVariant != 0) {
            if (step == 2) {
                step = 4;
            }
            if (step == 3) {
                step = 5;
            }
        }
        return step;
    case 1:
        if (self->comboAirVariant != 0) {
            if (step == 2) {
                step = 4;
            }
            if (step == 3) {
                step = 5;
            }
        }
        return step + 6;
    case 2:
        return step + 0xc;
    case 3:
        return step + 0x10;
    case 4:
    case 5:
        return 0x14;
    case 6:
    case 7:
        if (attackId > 5 && attackId < 0x16) {
            return 0x15;
        }
        return 0x14;
    default:
        return -1;
    }
}
