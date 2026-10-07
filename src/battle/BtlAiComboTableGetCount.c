// bdc 0x088998a8 BtlAiComboTableGetCount
#include "bdc.h"

/* Returns the number of entries of a BtlAi combo-input table (BtlAiComboTable::count). */
s32 BtlAiComboTableGetCount(BtlAiComboTable *table)
{
    return table->count;
}
