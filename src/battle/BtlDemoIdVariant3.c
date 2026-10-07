// bdc 0x089053a8 BtlDemoIdVariant3
#include "bdc.h"

/* Returns the demo id of variant 3 for brawler `n` (1..20): `n * 4 + 0x16`; 0x1a otherwise. */
int BtlDemoIdVariant3(int n)
{
    if (n > 0 && n < 0x15) {
        return n * 4 + 0x16;
    }
    return 0x1a;
}
