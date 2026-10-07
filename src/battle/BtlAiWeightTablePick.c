// bdc 0x08a2a29c BtlAiWeightTablePick
#include "bdc.h"

/* Weighted random pick from a `BtlAiWeightTable`: when `weights` is set and the cached sum is
   not yet valid, sums the `count` weights into `total` and marks it valid; then, if `total` is
   non-zero, draws `r = ``CoreRandNext``(total)` and returns the index of the first entry whose
   running sum exceeds `r` (signed compare), or `count` if none does. Returns -1 when `total` is 0. */
s32 BtlAiWeightTablePick(BtlAiWeightTable *self)
{
    s32 r;
    s32 sum;
    u32 i;

    if (self->weights != NULL && self->totalValid == 0) {
        self->total = 0;
        for (i = 0; i < self->count; i++) {
            self->total += self->weights[i];
        }
        self->totalValid = 1;
    }
    if (self->total == 0) {
        return -1;
    }
    r = (s32)CoreRandNext(self->total);
    sum = 0;
    for (i = 0; i < self->count; i++) {
        sum += self->weights[i];
        if (r < sum) {
            break;
        }
    }
    return (s32)i;
}
