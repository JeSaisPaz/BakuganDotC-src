// bdc 0x089bbf9c BootIsThreadSleeping
#include "bdc.h"

/* Returns whether the game thread in `g_threadTable` slot `index` is asleep: true when
   `sceKernelReferThreadStatus` reports status WAIT (4) with wait type 1 (sleep), otherwise the
   slot's sleeping flag (byte `+0x1c`, set by `BootSleepCurrentThread`). */

u8 BootIsThreadSleeping(int index)

{
  SceUID thid;
  int result;
  u8 sleeping;
  SceKernelThreadInfo info;
  
  sleeping = '\0';
  thid = BootGetThreadId(index);
  if (0 < thid) {
    info.size = 0x6c;
    result = sceKernelReferThreadStatus(thid,&info);
    if (result == 0) {
      switch(info.status) {
      case 1:
      case 2:
        break;
      case 4:
        if (info.waitType == 1) {
          sleeping = '\x01';
        }
      }
    }
  }
  if (sleeping == '\0') {
    sleeping = (u8)g_threadTable[index].unk1c;
  }
  return sleeping;
}

