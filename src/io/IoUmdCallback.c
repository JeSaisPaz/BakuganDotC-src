// bdc 0x089fb43c IoUmdCallback
#include "bdc.h"

/* UMD state callback created by `IoUmdInit` (`sceKernelCreateCallback`, registered with
   `sceUmdRegisterUMDCallBack`): stores the UMD state flags in `g_umdState`, updates the
   disc-present byte `g_umdDiscPresent` (1 on `PSP_UMD_PRESENT` 0x02, 0 on `PSP_UMD_NOT_PRESENT` 0x01) and
   the drive-ready byte `g_umdDriveReady` (1 on `PSP_UMD_READY` 0x20, 0 on `INITED` 0x10, `INITING` 0x08
   or `CHANGED` 0x04). Returns 0. */

int IoUmdCallback(int count, int state, void *arg)

{
  g_umdState = state;
  if ((state & 2U) == 0) {
    if ((state & 1U) != 0) {
      g_umdDiscPresent = '\0';
    }
  }
  else {
    g_umdDiscPresent = '\x01';
  }
  if ((state & 0x20U) == 0) {
    if ((state & 0x10U) == 0) {
      if ((state & 8U) != 0) {
        g_umdDriveReady = '\0';
      }
    }
    else {
      g_umdDriveReady = '\0';
    }
  }
  else {
    g_umdDriveReady = '\x01';
  }
  if ((state & 4U) != 0) {
    g_umdDriveReady = '\0';
  }
  return 0;
}

