// bdc 0x088995f4 BtlAiComboTableCtor
#include "bdc.h"

/* Constructor of the combo-input table of `BtlAi`: installs
   `g_btlAiComboTableVtbl`, clears the owner kind and the entry count. Returns `table`. Called by
   `BtlAiCtor`. */
BtlAiComboTable *BtlAiComboTableCtor(BtlAiComboTable *table)
{
    table->vtbl = g_btlAiComboTableVtbl;
    table->ownerKind = 0;
    table->count = 0;
    return table;
}
