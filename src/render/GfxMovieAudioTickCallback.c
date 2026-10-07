// bdc 0x089d54f0 GfxMovieAudioTickCallback
#include "bdc.h"

/* Callback registered with `GfxSetVblankHandler(0, cb, 0)` when a movie starts (`GfxMovieOpenThread`)
   and cleared by `GfxMoviePlayerRequestStop`: if the movie player exists, runs
   `GfxMoviePlayerUpdate` on it. */

void GfxMovieAudioTickCallback(void)

{
  GfxMoviePlayer *player;

  if (GfxMovieHasPlayer()) {
    player = GfxMovieGetPlayer();
    GfxMoviePlayerUpdate(player);
  }
  return;
}
