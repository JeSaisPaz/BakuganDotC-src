// bdc 0x08897e50 BtlAiGetComboInput
#include "bdc.h"

/* Returns the s16 input value of step `step` of combo entry `index` of `BtlAi`
   (`BtlAiComboTableGetInputs`). Used by `BtlAiExecComboAttack` to replay a combo on the virtual
   pad. */

s32 BtlAiGetComboInput(BtlAi *self, s32 index, s32 step)
{
  s16 *inputs = BtlAiComboTableGetInputs(&self->comboTable, index);
  return inputs[step];
}
