// bdc 0x089bbf38 BootWakeupThread
#include "bdc.h"

/* Wakes the game thread in `g_threadTable` slot `index` with `sceKernelWakeupThread` and clears
   its sleeping flag (byte `+0x1c`). Returns 1 on success, 0 when the thread does not exist or the
   wakeup failed. */

int BootWakeupThread(int index)
{
  SceUID thid;
  int ok;

  ok = 0;
  thid = BootGetThreadId(index);
  if ((0 < thid) && (sceKernelWakeupThread(thid) == 0)) {
    ok = 1;
    *(u8 *)&g_threadTable[index].unk1c = 0;
  }
  return ok;
}
