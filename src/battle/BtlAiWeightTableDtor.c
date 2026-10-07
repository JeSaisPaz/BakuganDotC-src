// bdc 0x08a2a1ec BtlAiWeightTableDtor
#include "bdc.h"

/* Destructor of the AI's weighted-choice table (picked from by `BtlAiWeightTablePick`): when the
   weight array is borrowed, first drops the pointer and clears `borrowed` (so nothing is freed);
   otherwise frees the array under `MemLock`. Then clears `count`, the cached `total` and
   `totalValid`, and frees the table itself when `flags & 1`. Does nothing for NULL. */
void BtlAiWeightTableDtor(BtlAiWeightTable *self, u32 flags)
{
    if (self == NULL) {
        return;
    }
    if (self->borrowed != 0) {
        self->weights = NULL;
        self->borrowed = 0;
    }
    if (self->weights != NULL) {
        s32 *weights = self->weights;

        MemLock();
        MemFree(weights, NULL, 0);
        MemUnlock();
        self->weights = NULL;
    }
    self->count = 0;
    self->total = 0;
    self->totalValid = 0;
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
