// bdc 0x089bc25c BootBackgroundProcessThread
#include "bdc.h"

/* Entry of the "MyThread-BackGroundProcess" thread (slot 17 of `g_threadTable`): creates the
   background-process worker (`CoreBackgroundProcessCreate`), runs its job loop on it
   (`CoreBackgroundProcessRun`, which never returns in practice), then destroys it
   (`CoreBackgroundProcessDestroy`) and returns 0. */

s32 BootBackgroundProcessThread(void)

{
  CoreBackgroundProcess *proc;
  
  CoreBackgroundProcessCreate();
  proc = CoreGetBackgroundProcess();
  CoreBackgroundProcessRun(proc);
  CoreBackgroundProcessDestroy();
  return 0;
}

