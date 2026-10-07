// bdc 0x089fb4c8 IoUmdInit
#include "bdc.h"

/* Initialises UMD access for the disc thread (`BootDiscThread`): creates a UMD callback
   (`IoUmdCallback`, id in `g_umdCallbackId`) and registers it, waits for a disc when none is
   present, activates `"disc0:"`, waits for drive state 0x20 (ready), prohibits disc replacement and
   checks the media (`IoUmdIsMediaReady`). */

void IoUmdInit(void)

{
  g_umdCallbackId = sceKernelCreateCallback("", IoUmdCallback, (void *)0x0);
  sceUmdRegisterUMDCallBack(g_umdCallbackId);
  if (sceUmdCheckMedium() == 0) {
    sceUmdWaitDriveStat(2);
  }
  sceUmdActivate(1, "disc0:");
  sceUmdWaitDriveStat(0x20);
  sceUmdReplaceProhibit();
  IoUmdIsMediaReady();
  return;
}
