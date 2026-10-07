// bdc 0x0886ac38 BtlBakuganGetComboStepMotionSet
#include "bdc.h"

/* Returns the motion set of the current melee combo step (`combos[comboIndex][comboStep]`): the
   step's `airMotionSet` when `comboAirVariant` is set, else `motionSet`. NULL when there is no
   combo or the step index is -1. */
void *BtlBakuganGetComboStepMotionSet(BtlBakugan *self)
{
    BtlComboStep *steps = self->combos[self->comboIndex];

    if (steps != NULL && self->comboStep != -1) {
        if (self->comboAirVariant != 0) {
            return steps[self->comboStep].airMotionSet;
        }
        return steps[self->comboStep].motionSet;
    }
    return NULL;
}
