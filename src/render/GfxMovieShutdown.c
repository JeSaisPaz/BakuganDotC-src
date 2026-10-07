// bdc 0x089d56d8 GfxMovieShutdown
#include "bdc.h"

/* Movie system shutdown (from `GfxMovieTaskDtor`): releases `g_moviePlayer`
   (`GfxMoviePlayerRelease`) and the block `g_movieBlock`, then requests the unload of the three
   movie modules in reverse order (`CoreModuleRequestUnload`, retrying every 200 us) and polls
   every 200 us until all three report unloaded (`CoreModuleIsUnloaded`). */

void GfxMovieShutdown(void)
{
  int i;
  int pending;

  if (g_moviePlayer != NULL) {
    GfxMoviePlayerRelease(g_moviePlayer, 3);
    g_moviePlayer = NULL;
  }
  if (g_movieBlock != NULL) {
    MemLock();
    MemFree(g_movieBlock, NULL, 0);
    MemUnlock();
    g_movieBlock = NULL;
  }
  for (i = 2; i >= 0; i--) {
    while (CoreModuleRequestUnload(CoreGetModuleMgr(), g_movieModuleSlots[i]) == 0) {
      sceKernelDelayThreadCB(200);
    }
  }
  do {
    sceKernelDelayThreadCB(200);
    pending = 0;
    for (i = 0; i < 3; i++) {
      if (CoreModuleIsUnloaded(CoreGetModuleMgr(), g_movieModuleSlots[i]) == 0) {
        pending++;
      }
    }
  } while (pending != 0);
}
