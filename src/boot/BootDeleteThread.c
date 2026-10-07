// bdc 0x089bbd40 BootDeleteThread
#include "bdc.h"

/* Deletes the game thread in `g_threadTable` slot `index` and returns 1 when the slot is free
   afterwards; `index >= 19` deletes all 19 slots, retrying until every one succeeds. */

int BootDeleteThread(int index)
{
  int ok;
  int i;
  int tid;
  int ret;
  BootThreadDef *slot;

  ok = 1;
  if (index < 0x13) {
    slot = &g_threadTable[index];
    tid = slot->threadId;
    if (0 < tid) {
      ret = sceKernelGetThreadExitStatus(tid);
      if (ret == 0x800201a4) {
        ok = 0;
        ret = sceKernelTerminateThread(tid);
        if (ret == 0x80020197) {
          slot->threadId = -1;
          ok = 1;
          sceKernelExitDeleteThread(1);
        }
      }
      else {
        ret = sceKernelDeleteThread(tid);
        if (ret == 0) {
          slot->threadId = -1;
        }
        else {
          ok = 0;
        }
      }
    }
  }
  else {
    do {
      ok = 1;
      i = 0;
      do {
        ret = BootDeleteThread(i);
        i = i + 1;
        if (ret == 0) {
          ok = 0;
          break;
        }
      } while (i < 0x13);
    } while (ok == 0);
  }
  return ok;
}
