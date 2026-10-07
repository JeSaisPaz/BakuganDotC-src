// bdc 0x089fa170 IoDiscSetPowerSuspend
#include "bdc.h"

/* Sets the power-suspend flag of the `CODiscSimple` disc reader (`g_discSimple`) (no lock);
   called by `BootPowerHandleFlags` on suspend/standby. */

void IoDiscSetPowerSuspend(IoDiscSimple *self)
{
  self->powerSuspend = 1;
}
