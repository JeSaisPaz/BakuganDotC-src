// bdc 0x089bbc7c BootStartThread
#include "bdc.h"

/* Creates and starts the thread described by slot `index` of `g_threadTable` (a
   `BootThreadDef`), unless that slot already holds a live thread (`threadId != -1`) or `index >=
   0x13`. Passes `arglen`/`argp` to the new thread, stores its UID in the slot and returns 1;
   returns 0 on any failure (a thread that was created but could not be started is deleted again).
    */

int BootStartThread(int index, void *argp, SceSize arglen)
{
  SceUID thid;
  int started;
  int result;

  result = 0;
  if (index < 0x13) {
    if ((g_threadTable[index].threadId == -1) &&
       (thid = sceKernelCreateThread
                         (g_threadTable[index].name,g_threadTable[index].entry,
                          g_threadTable[index].initPriority,g_threadTable[index].stackSize,
                          g_threadTable[index].attr,g_threadTable[index].option), 0 < thid)) {
      started = sceKernelStartThread(thid,arglen,argp);
      if (started == 0) {
        g_threadTable[index].threadId = thid;
        result = 1;
      }
      else {
        sceKernelDeleteThread(thid);
      }
    }
  }
  return result;
}
