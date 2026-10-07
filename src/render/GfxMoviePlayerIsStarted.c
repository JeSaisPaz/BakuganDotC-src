// bdc 0x089d5fd8 GfxMoviePlayerIsStarted
#include "bdc.h"

/* Returns the "started" byte `+0x4d`, set by `GfxMovieOpenThread` once the PSMF player runs;
   polled by state 2 of `GfxMovieTaskUpdate`. */

bool GfxMoviePlayerIsStarted(GfxMoviePlayer *player)

{
  return player->started != '\0';
}

