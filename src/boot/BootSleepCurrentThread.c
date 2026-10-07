// bdc 0x089bbeb8 BootSleepCurrentThread
#include "bdc.h"

/* Puts the calling game thread to sleep with `sceKernelSleepThreadCB`, flagging its
   `g_threadTable` slot as sleeping (byte `+0x1c` = 1) while it waits; the flag is cleared again
   if the sleep call returns an error. */

void BootSleepCurrentThread(void)
{
  s32 tid;
  BootThreadDef *slot;
  BootThreadDef *found;
  int i;

  tid = sceKernelGetThreadId();
  found = NULL;
  if (tid >= 0) {
    i = 0;
    slot = g_threadTable;
    do {
      i++;
      if (slot->threadId == tid) {
        *(u8 *)&slot->unk1c = 1;
        found = slot;
        break;
      }
      slot++;
    } while (i < 0x13);
  }
  if (sceKernelSleepThreadCB() != 0 && found != NULL) {
    *(u8 *)&found->unk1c = 0;
  }
}
