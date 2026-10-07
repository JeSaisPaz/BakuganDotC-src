// bdc 0x089d5dac GfxMoviePlayerDraw
#include "bdc.h"

/* Draw step from `GfxMovieTaskDraw`: if active, not stopping and the last video fetch returned 0,
   draws the frame (`GfxMoviePlayerDrawFrame`); then stores the current PTS
   (`scePsmfPlayerGetCurrentPts`) in `+0x54` (used for the skip delay). */

void GfxMoviePlayerDraw(GfxMoviePlayer *player)
{
  u32 pts;

  if (GfxMoviePlayerIsActive(player) != 0 && !GfxMoviePlayerIsStopping(player)) {
    if (player->videoResult >= -0x7f9e9ff3 && player->videoResult == 0) {
      GfxMoviePlayerDrawFrame(player);
    }
  }
  if (player->videoResult == 0 && player->playInfo != NULL && player->psmf != NULL &&
      scePsmfPlayerGetCurrentPts(player->psmf, &pts) == 0) {
    player->pts = pts;
  }
}
