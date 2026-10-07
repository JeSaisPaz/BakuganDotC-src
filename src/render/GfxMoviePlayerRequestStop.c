// bdc 0x089d618c GfxMoviePlayerRequestStop
#include "bdc.h"

/* If the player is active, starts game thread 14 (the stop thread, which runs
   `GfxMoviePlayerStopStep`) and unregisters the movie tick callback (`GfxSetVblankHandler(0,
   NULL, 0)`); returns 1 when the thread was started. */

bool GfxMoviePlayerRequestStop(GfxMoviePlayer *player)
{
  bool stopped;

  stopped = false;
  if (player->active != 0 && BootStartThread(0xe, (void *)0, 0) != 0) {
    stopped = true;
    GfxSetVblankHandler(0, (void *)0, (void *)0);
  }
  return stopped;
}
