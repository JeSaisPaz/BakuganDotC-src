// bdc 0x0886b308 BtlBakuganMatchComboStepInput
#include "bdc.h"

/* Checks the input recorded for the current combo step (signed byte `comboInputs[comboStep]`)
   against the step's expected input (`BtlComboStep` `input`, or `airInput` when
   `comboAirVariant` is set): returns 0 on a match; 1 when, not in the air variant, the step's
   `branch` flag is set and the input equals `airInput` instead; else -1. */
int BtlBakuganMatchComboStepInput(BtlBakugan *self)
{
    const BtlComboStep *step = &self->combos[self->comboIndex][self->comboStep];
    s32 recorded = (s8)self->comboInputs[self->comboStep];
    s32 expected;

    if (self->comboAirVariant == 0) {
        expected = step->input;
    } else {
        expected = step->airInput;
    }
    if (recorded == expected) {
        return 0;
    }
    if (self->comboAirVariant == 0 && step->branch != 0 && recorded == step->airInput) {
        return 1;
    }
    return -1;
}
