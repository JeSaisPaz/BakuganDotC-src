// bdc 0x08a2a174 BtlCpuUnitIsCpuUnit
#include "bdc.h"

/* Returns 1 for the "is a CPU-controlled unit" battle-unit class test (virtual slot 13, `+0x6c`) in
   BtlCpuUnit's vtables. */

int BtlCpuUnitIsCpuUnit(void *unit)
{
  (void)unit;
  return 1;
}
