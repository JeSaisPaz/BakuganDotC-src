// bdc 0x08905410 BtlDemoIdVariant2
#include "bdc.h"

/* Returns the demo id of variant 2 for brawler `n` (1..20): `n * 4 + 0x15`; 0x19 otherwise. */

int BtlDemoIdVariant2(int n)

{
  if ((0 < n) && (n < 0x15)) {
    return n * 4 + 0x15;
  }
  return 0x19;
}

