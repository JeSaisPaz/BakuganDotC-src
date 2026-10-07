// bdc 0x08897e34 BtlAiGetComboLength
#include "bdc.h"

/* Returns the number of input steps of combo entry `index` of the AI's combo table
   (`BtlAiComboTableGetLength`). */
s32 BtlAiGetComboLength(BtlAi *self, s32 index)
{
    return BtlAiComboTableGetLength(&self->comboTable, index);
}
