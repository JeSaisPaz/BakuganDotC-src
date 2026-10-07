// bdc 0x0888ce68 BtlAiChannelSetRules
#include "bdc.h"

/* Attaches the rule group `rules` (a `BtlAiRuleGroup`) to a command channel of
   `BtlAi` and rebuilds its ten weight tables: for each, drops a borrowed array
   (clears the pointer and `borrowed`) or frees an owned one, clears `count`, `total` and
   `totalValid`, then, when the group has records, allocates `count` words from the low end of the
   heap (`MemSetAllocFromLow`, restoring the previous policy) and zeroes them. */
void BtlAiChannelSetRules(BtlAiChannel *self, void *rules)
{
    BtlAiWeightTable *table = self->weights;
    int t;

    self->rules = rules;
    for (t = 0; t < 10; t++, table++) {
        u32 count = (u32)((BtlAiRuleGroup *)self->rules)->count;
        u32 i;

        if (table->borrowed != 0) {
            table->weights = NULL;
            table->borrowed = 0;
        }
        if (table->weights == NULL) {
            table->count = 0;
        } else {
            MemLock();
            MemFree(table->weights, NULL, 0);
            MemUnlock();
            table->weights = NULL;
            table->count = 0;
        }
        table->total = 0;
        table->totalValid = 0;
        if (count != 0) {
            bool fromLow;
            s32 *weights;

            table->count = count;
            MemLock();
            fromLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            weights = MemAlloc(count << 2, NULL, 0);
            MemSetAllocFromLow(fromLow);
            MemUnlock();
            table->weights = weights;
            for (i = 0; i < table->count; i++) {
                table->weights[i] = 0;
            }
        }
    }
}
