// bdc 0x0888cb64 BtlAiChannelDtor
#include "bdc.h"

/* Destructor of a 300-byte command channel of `BtlAi`: for a non-NULL channel, empties
   each of its ten `BtlAiWeightTable` records `weights` (a borrowed weight array is only detached,
   an owned one is freed with `MemFree` under `MemLock`; count, total and totalValid are
   cleared), destroys the record array (`CxxVecDelete` with `BtlAiWeightTableDtor`) and frees
   the channel when bit 0 of `flags` is set. */
void BtlAiChannelDtor(BtlAiChannel *self, u32 flags)
{
    BtlAiWeightTable *table;
    s32 *weights;
    int i;

    if (self == NULL) {
        return;
    }
    table = self->weights;
    for (i = 0; i < 10; i++, table++) {
        if (table->borrowed != 0) {
            table->weights = NULL;
            table->borrowed = 0;
        }
        weights = table->weights;
        if (weights != NULL) {
            MemLock();
            MemFree(weights, NULL, 0);
            MemUnlock();
            table->weights = NULL;
        }
        table->count = 0;
        table->total = 0;
        table->totalValid = 0;
    }
    CxxVecDelete(self->weights, 10, 0x10, BtlAiWeightTableDtor, 0, 0);
    if (flags & 1) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
