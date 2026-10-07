// bdc 0x0885bd3c BtlUnitAltUpdate
#include "bdc.h"

/* Slot 7 (update) of `BtlUnitAlt`: forwards to the CPU-unit base update
   `BtlCpuUnitUpdate`. */
void BtlUnitAltUpdate(void *unit)
{
    BtlCpuUnitUpdate(unit);
}
