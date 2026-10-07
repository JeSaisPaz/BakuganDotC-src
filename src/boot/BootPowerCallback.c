// bdc 0x089bb34c BootPowerCallback
#include "bdc.h"

/* PSP power callback ("MyCB-Power"): saves the `PSP_POWER_CB_*` flag word in
   `g_powerCallbackFlags` and hands it to `BootPowerHandleFlags`, which reacts to
   suspend/resume/standby. Always returns 0. */

int BootPowerCallback(int count, int powerFlags, void *common)

{
  g_powerCallbackFlags = powerFlags;
  BootPowerHandleFlags(powerFlags);
  return 0;
}

