// bdc 0x089fa170 IoDiscSetPowerSuspend
#include "bdc.h"

/* Sets the power-suspend flag of the `CODiscSimple` disc reader (`g_discSimple`) (no lock);
   called by `BootPowerHandleFlags` only on SUSPENDING (`0x10000`), not when STANDBY (`0x80000`)
   is set. */

void IoDiscSetPowerSuspend(IoDiscSimple *self)
{
  self->powerSuspend = 1;
}
