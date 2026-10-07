// bdc 0x0886b38c BtlBakuganGetNextComboStepMotionSet
#include "bdc.h"

/* Returns the motion set of the step after the current one (the air variant's when the unit is
   airborne), i.e. what `BtlBakuganGetComboStepMotionSet` will return after advancing; NULL
   without a combo. */
void *BtlBakuganGetNextComboStepMotionSet(BtlBakugan *self)
{
    BtlComboStep *steps = self->combos[self->comboIndex];
    if (steps == NULL) {
        return NULL;
    }
    if (self->comboAirVariant != 0) {
        return steps[self->comboStep + 1].airMotionSet;
    }
    return steps[self->comboStep + 1].motionSet;
}
