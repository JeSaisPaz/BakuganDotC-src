// bdc 0x089fb544 IoUmdShutdown
#include "bdc.h"

/* Unregisters and deletes the UMD callback created by `IoUmdInit` (`g_umdCallbackId`). */

void IoUmdShutdown(void)

{
  sceUmdUnRegisterUMDCallBack(g_umdCallbackId);
  sceKernelDeleteCallback(g_umdCallbackId);
  return;
}
