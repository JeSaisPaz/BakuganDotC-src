// bdc 0x088997d4 BtlAiComboTableInit
#include "bdc.h"

/* Resets the combo-input table (`BtlAiComboTable`) of `BtlAi` for `unit`: stores
   the unit's kind (its `CoreObject` `unk08`) as `ownerKind`, clears `count` and the group,
   length and eight inputs of all 16 entries, and for kinds below 0x15 builds it
   (`BtlAiComboTableBuild`). */
void BtlAiComboTableInit(BtlAiComboTable *table, void *unit)
{
    int i;
    int j;

    table->count = 0;
    table->ownerKind = ((CoreObject *)unit)->unk08;
    for (i = 0; i < 16; i++) {
        table->entries[i].group = 0;
        table->entries[i].length = 0;
        for (j = 0; j < 8; j++) {
            table->entries[i].inputs[j] = 0;
        }
    }
    if (table->ownerKind < 0x15) {
        BtlAiComboTableBuild(table, unit);
    }
}
