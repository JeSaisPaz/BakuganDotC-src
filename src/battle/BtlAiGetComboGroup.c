// bdc 0x08897e14 BtlAiGetComboGroup
#include "bdc.h"

/* Returns the group (combo number 0..5 of the species data) of combo entry `index` of
   `BtlAi` (`BtlAiComboTableGetEntry` word 0). */
s32 BtlAiGetComboGroup(BtlAi *self, s32 index)
{
    return BtlAiComboTableGetEntry(&self->comboTable, index)->group;
}
