// bdc 0x089bc43c BootMovieStopThread
#include "bdc.h"

/* Entry of thread slot 14 "MyThread-MovieStop": if a movie player exists (`GfxMovieHasPlayer`),
   keeps calling `GfxMoviePlayerStopStep` on it once per vblank until `GfxMoviePlayerIsActive`
   reports it inactive. Returns 0. */

int BootMovieStopThread(void)
{
  if (GfxMovieHasPlayer()) {
    while (GfxMoviePlayerIsActive(GfxMovieGetPlayer())) {
      sceDisplayWaitVblankStartCB();
      GfxMoviePlayerStopStep(GfxMovieGetPlayer());
    }
  }
  return 0;
}
