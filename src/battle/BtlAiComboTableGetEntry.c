// bdc 0x08899858 BtlAiComboTableGetEntry
#include "bdc.h"

/* Returns entry `index` of the combo-input table of `BtlAi`. */
BtlAiComboEntry *BtlAiComboTableGetEntry(BtlAiComboTable *table, s32 index)
{
    return &table->entries[index];
}
