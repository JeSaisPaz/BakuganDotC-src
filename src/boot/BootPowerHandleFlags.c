// bdc 0x089bd040 BootPowerHandleFlags
#include "bdc.h"

/* Reacts to the `PSP_POWER_CB_*` flags passed by `BootPowerCallback`: ignored on STANDBY
   (`0x80000`); on SUSPENDING (`0x10000`) sets `CorePower``.resumeNotify` (when
   `CorePowerIsInitialized`) and pauses the disc manager (`IoDiscSetPowerSuspend`); on RESUMING
   (`0x20000`) sets `CorePower``.unk24 = 1`. */

void BootPowerHandleFlags(u32 powerFlags)
{
  if ((powerFlags & 0x80000) == 0) {
    if ((powerFlags & 0x10000) == 0) {
      if ((powerFlags & 0x20000) != 0) {
        CorePowerGet()->unk24 = 1;
      }
    }
    else {
      if (CorePowerIsInitialized() != 0) {
        CorePowerGet()->resumeNotify = 1;
      }
      if (IoDiscHasManager()) {
        IoDiscSetPowerSuspend(IoDiscGetManager());
      }
    }
  }
}
