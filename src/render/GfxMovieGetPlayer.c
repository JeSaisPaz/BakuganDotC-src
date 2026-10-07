// bdc 0x089d5800 GfxMovieGetPlayer
#include "bdc.h"

/* Returns the movie (PSMF) player object at `0x08ac5b80` (NULL before it is created). Callers pass
   it to the player helpers (`GfxMoviePlayerRequestOpen`, `GfxMoviePlayerIsStarted`, `GfxMoviePlayerTick`,
   `GfxMoviePlayerIsStopped`, ...). */

void *GfxMovieGetPlayer(void)

{
  return g_moviePlayer;
}

