// bdc 0x089d6974 GfxMovieTaskDtor
#include "bdc.h"

/* Destructor of the movie task (id 10040): sets sound decoder 0 back to mode 0, shuts the movie
   player down (`GfxMovieShutdown`), chains to `CoreTaskDestroy`. */

void GfxMovieTaskDtor(CoreTask *task, u32 flags)

{
  SndDecOut *dec;
  
  if (task != (CoreTask *)0x0) {
    task->vtable = g_gfxMovieTaskVtbl;
    dec = SndDecOutGet(0);
    SndDecOutSetMode(dec,0);
    GfxMovieShutdown();
    CoreTaskDestroy(task,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(task,NULL,0);
      MemUnlock();
    }
  }
  return;
}

