// bdc 0x08897df8 BtlAiGetComboCount
#include "bdc.h"

/* Returns the number of combo entries in the combo table of `BtlAi`
   (`BtlAiComboTableGetCount`). */

s32 BtlAiGetComboCount(BtlAi *self)
{
    return BtlAiComboTableGetCount(&self->comboTable);
}
