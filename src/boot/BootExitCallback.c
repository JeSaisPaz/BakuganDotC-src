// bdc 0x089bb320 BootExitCallback
#include "bdc.h"

/* PSP home-button exit callback (`"MyCB-ExitGame"`): calls `sceKernelExitGame` and returns 0. The
   kernel invokes it when the user chooses "Exit game"; registered by `BootRegisterCallbacks`
   through `sceKernelCreateCallback` + `sceKernelRegisterExitCallback`. */

int BootExitCallback(int count, int arg, void *common)

{
  sceKernelExitGame();
  return 0;
}

