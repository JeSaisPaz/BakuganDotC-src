// bdc 0x089bb374 BootRegisterCallbacks
#include "bdc.h"

/* Registers the two kernel callbacks every PSP game needs: "MyCB-ExitGame" (`BootExitCallback`,
   which just calls `sceKernelExitGame`) through `sceKernelRegisterExitCallback` and
   "MyCB-Power" (`BootPowerCallback`) through `scePowerRegisterCallback` with slot -1. Returns 1
   on success, 0 if any step fails. */

int BootRegisterCallbacks(void)

{
  int ok;
  SceUID exitCb;
  SceUID powerCb;

  ok = 1;
  exitCb = sceKernelCreateCallback("MyCB-ExitGame", BootExitCallback, (void *)0x0);
  if (exitCb < 0) {
    ok = 0;
  }
  else if (sceKernelRegisterExitCallback(exitCb) != 0) {
    ok = 0;
  }
  if (ok != 0) {
    powerCb = sceKernelCreateCallback("MyCB-Power", BootPowerCallback, (void *)0x0);
    if (powerCb < 0) {
      ok = 0;
    }
    else if (scePowerRegisterCallback(-1, powerCb) != 0) {
      ok = 0;
    }
  }
  return ok;
}
