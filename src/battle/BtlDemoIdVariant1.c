// bdc 0x089053cc BtlDemoIdVariant1
#include "bdc.h"

/* Returns the demo id of variant 1 for brawler `n` (1..20): `n * 4 + 0x14`; 0x18 otherwise. */
int BtlDemoIdVariant1(int n)
{
    if (n > 0 && n < 0x15) {
        return n * 4 + 0x14;
    }
    return 0x18;
}
