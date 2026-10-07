// bdc 0x089bc3d4 BootMovieStartThread
#include "bdc.h"

/* Entry of thread slot 13 "MyThread-MovieStart" (thread table at `0x08ac4f7c`, 0x20-byte entries
   `{name, entry, priority 0x16, stack 0x2800, …}`): while a movie player exists
   (`GfxMovieHasPlayer`), waits a vblank per try until `GfxMoviePlayerHasPendingRequest(player)`
   reports ready, then opens the movie (`GfxMovieOpenThread` on `GfxMovieGetPlayer`). Returns 0. */

int BootMovieStartThread(void)
{
  if (GfxMovieHasPlayer()) {
    do {
      if (!GfxMovieHasPlayer()) {
        return 0;
      }
      sceDisplayWaitVblankStartCB();
    } while (!GfxMoviePlayerHasPendingRequest(GfxMovieGetPlayer()));
    GfxMovieOpenThread(GfxMovieGetPlayer());
  }
  return 0;
}
