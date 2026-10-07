// bdc 0x088705e4 BtlKindsHaveDuplicate
#include "bdc.h"

/* Returns 1 when the kind list `kinds` (up to 4 entries, -1 terminated) contains a kind 1..0x14
   twice, else 0 (`BtlCountKindsHasDuplicate` into a scratch count array). Called by
   `BtlMainPhaseLoad`. */
int BtlKindsHaveDuplicate(int *kinds)
{
    u8 counts[0x15];

    if (BtlCountKindsHasDuplicate(kinds, counts) != 0) {
        return 1;
    }
    return 0;
}
