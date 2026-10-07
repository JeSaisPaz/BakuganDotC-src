// bdc 0x089078e0 BtlDemoScbEventGroupType
#include "bdc.h"

/* Maps the header byte pair of a `.scb` event group (`hi`, `lo`) to the group type used by
   `BtlDemoScbEventGroupReadEvents`: `hi` 1 gives 1, `hi` 0xff gives 2; `hi` 0 gives 3 for `lo` 0
   and 4..8 for `lo` 1..5; every other combination (including `hi` 0 with `lo` >= 6) gives 0.
   `group` is unused. */
int BtlDemoScbEventGroupType(void *group, int hi, u32 lo)
{
  (void)group;
  if (hi == 1) {
    return 1;
  }
  if (hi == 0xff) {
    return 2;
  }
  if (hi != 0 || lo >= 6) {
    return 0;
  }
  switch (lo) {
  case 1:
    return 4;
  case 2:
    return 5;
  case 3:
    return 6;
  case 4:
    return 7;
  case 5:
    return 8;
  default:
    return 3;
  }
}
