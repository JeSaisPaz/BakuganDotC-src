// bdc 0x089d6170 GfxMoviePlayerSetStopping
#include "bdc.h"

/* Sets the stopping byte `+0x41` and clears the frame counters `+0x44`/`+0x48`. */

void GfxMoviePlayerSetStopping(GfxMoviePlayer *player, bool stopping)

{
  player->stopping = stopping;
  player->firstFrame = 0;
  player->gotFrame = '\0';
  return;
}

