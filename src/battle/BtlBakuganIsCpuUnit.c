// bdc 0x08a29ed8 BtlBakuganIsCpuUnit
#include "bdc.h"

/* Returns 0 for the "is a CPU-controlled unit" battle-unit class test (virtual slot 13, `+0x6c`) in
   BtlBakugan's vtables. */

int BtlBakuganIsCpuUnit(BtlBakugan *self)

{
  (void)self;
  return 0;
}

