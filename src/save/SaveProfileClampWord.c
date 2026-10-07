// bdc 0x0880ccd8 SaveProfileClampWord
#include "bdc.h"

/* Clamps word `index` of the profile's word table (`self->words`) into the allowed range: the lower
   bound comes from `SaveProfileWordMin(self, index)` and the upper bound from
   `SaveProfileWordMax(self, index)` (both per-index); a value below the minimum is replaced by it, a
   value above the maximum by the maximum, an in-range value is rewritten unchanged. Called by
   `SaveProfileSetWord` after every store. */

void SaveProfileClampWord(SaveProfile *self, s32 index)
{
    s32 max = SaveProfileWordMax(self, index);
    s32 min = SaveProfileWordMin(self, index);
    u32 *word = self->words + index;
    s32 v = *word;

    if (v < min) {
        *word = min;
        return;
    }
    if (max < v) {
        *word = max;
        return;
    }
    *word = v;
}
