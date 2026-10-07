// bdc 0x0880d50c SaveProfileCounterMin
#include "bdc.h"

/* Lower bound of profile counter `idx`: always 0 (both arguments are ignored). Part of the clamp
   used by `SaveProfileClampCounter`. */
s32 SaveProfileCounterMin(SaveProfile *self, s32 idx)
{
    (void)self;
    (void)idx;
    return 0;
}
