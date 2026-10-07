// bdc 0x089bbe2c BootIsThreadRunning
#include "bdc.h"

/* Returns 1 when slot `index` of `g_threadTable` holds a live thread that has not terminated yet,
   else 0: `sceKernelGetThreadExitStatus` returns `SCE_KERNEL_ERROR_NOT_DORMANT` (0x800201A4) for
   a thread that is still running. Out-of-range indices (>= 0x13) and slots without a thread id
   (`threadId <= 0`) give 0. */

int BootIsThreadRunning(int index)

{
  int status;
  
  if (((index < 0x13) && (0 < g_threadTable[index].threadId)) &&
     (status = sceKernelGetThreadExitStatus(g_threadTable[index].threadId), status == -0x7ffdfe5c)) {
    return 1;
  }
  return 0;
}

