// bdc 0x0886b2b4 BtlBakuganGetComboLength
#include "bdc.h"

/* Returns the number of steps of the current melee combo (`combos[comboIndex]`): counts
   `BtlComboStep`s until one has neither an air nor a ground motion set. Returns 0 when there is
   no combo, and also when 10 steps all have a motion set (the loop falls through to 0). */
int BtlBakuganGetComboLength(BtlBakugan *self)
{
    const BtlComboStep *step = self->combos[self->comboIndex];
    int count = 0;

    if (step != NULL) {
        do {
            if (step->airMotionSet == NULL && step->motionSet == NULL) {
                return count;
            }
            count++;
            step++;
        } while (count < 10);
    }
    return 0;
}
