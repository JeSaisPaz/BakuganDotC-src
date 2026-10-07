// bdc 0x089d6128 GfxMoviePlayerIsStopped
#include "bdc.h"

/* Returns 1 when the movie player has stopped: its playing byte is 0 (`GfxMoviePlayerIsActive`)
   and the movie-stop thread (game thread slot 14, `BootMovieStopThread`) no longer exists
   (`BootGetThreadId``(0xe) == -1`). */

bool GfxMoviePlayerIsStopped(GfxMoviePlayer *player)

{
  bool stopped;

  stopped = false;
  if (GfxMoviePlayerIsActive(player) == 0 && BootGetThreadId(0xe) == -1) {
    stopped = true;
  }
  return stopped;
}

