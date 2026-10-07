// bdc 0x0889988c BtlAiComboTableGetLength
#include "bdc.h"

/* Returns the input count of entry `index` of the combo-input table of `BtlAi`. */
s32 BtlAiComboTableGetLength(BtlAiComboTable *table, s32 index)
{
    return table->entries[index].length;
}
