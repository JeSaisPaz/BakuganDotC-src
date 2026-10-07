// bdc 0x0889966c BtlAiComboTableBuild
#include "bdc.h"

/* Fills the combo-input table (`BtlAiComboTable`) of `BtlAi` from the six combos
   of the unit's kind (`g_btlKindComboTables`, kind = the unit's `CoreObject` `unk08`). For each
   combo it appends, at `count`, an entry with the plain ground inputs of the steps up to the
   first step without a ground motion set, counting the branch steps; then, for each branch
   number n from that count down to 1, an entry that follows the ground inputs until the n-th
   branch step and the air inputs (while an air motion set exists) from there on. Every entry
   records its combo number (`group`) and its `length`. Neither the entry count nor the input
   index is bounds-checked. */
void BtlAiComboTableBuild(BtlAiComboTable *table, void *unit)
{
    BtlComboStep **combos = g_btlKindComboTables[((CoreObject *)unit)->unk08];
    BtlAiComboEntry *entry = &table->entries[table->count];
    int branches = 0;
    int combo;

    for (combo = 0; combo < 6; combo++, combos++) {
        const BtlComboStep *step = *combos;
        int count = table->count;
        int i;

        entry->group = combo;
        for (i = 0;; i++, step++) {
            if (step->motionSet == NULL) {
                entry->length = i;
                table->count = count + 1;
                entry++;
                break;
            }
            if (step->branch != 0) {
                branches++;
            }
            entry->inputs[i] = step->input;
        }

        for (; branches != 0; branches--) {
            int remaining = branches;
            bool inAir = false;

            step = *combos;
            count = table->count;
            entry->group = combo;
            for (i = 0;; i++, step++) {
                bool end = true;

                if (!inAir && step->branch != 0 && --remaining == 0) {
                    inAir = true;
                }
                if (inAir) {
                    if (step->airMotionSet != NULL) {
                        entry->inputs[i] = step->airInput;
                        end = false;
                    }
                } else if (step->motionSet != NULL) {
                    entry->inputs[i] = step->input;
                    end = false;
                }
                if (end) {
                    entry->length = i;
                    table->count = count + 1;
                    entry++;
                    break;
                }
            }
        }
    }
}
