// bdc 0x089053f0 BtlDemoIdVariant0
#include "bdc.h"

/* Returns the demo id of variant 0 for brawler `n` (1..20): `n * 4 + 0x13`; 0x17 otherwise. */
int BtlDemoIdVariant0(int n)
{
    if (n > 0 && n < 0x15) {
        return n * 4 + 0x13;
    }
    return 0x17;
}
