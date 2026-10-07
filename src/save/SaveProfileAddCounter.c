// bdc 0x0880d5ec SaveProfileAddCounter
#include "bdc.h"

/* Adds `delta` to profile counter `idx` (0..2), clamps the sum with `SaveProfileClampCounter`,
   stores it with `SaveProfileSetCounter` and returns the new value (0 when the profile block is
   missing or `idx` is out of range). */

s32 SaveProfileAddCounter(SaveProfile *self, s32 idx, s32 delta)
{
    s32 value = 0;

    if (self->data != NULL && idx >= 0 && idx < 3) {
        value = delta + SaveProfileGetCounter(self, idx);
        SaveProfileClampCounter(self, idx, &value);
        SaveProfileSetCounter(self, idx, value);
    }
    return value;
}
