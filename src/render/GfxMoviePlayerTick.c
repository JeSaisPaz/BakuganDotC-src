// bdc 0x089d5a80 GfxMoviePlayerTick
#include "bdc.h"

/* Per-frame movie step called by `GfxMovieTaskUpdate`: fetches the next video frame
   (`GfxMoviePlayerFetchVideo`); returns 0 once the player is inactive and the stop thread (slot
   14) has ended (`BootDeleteThread(0xe)`), else 1. */

bool GfxMoviePlayerTick(GfxMoviePlayer *player)
{
  bool running;

  running = true;
  GfxMoviePlayerFetchVideo(player);
  if (GfxMoviePlayerIsActive(player) == 0 && BootDeleteThread(0xe) != 0) {
    running = false;
  }
  return running;
}
