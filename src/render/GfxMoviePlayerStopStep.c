// bdc 0x089d61e4 GfxMoviePlayerStopStep
#include "bdc.h"

/* Stop sequence step run from thread 14 (`BootMovieStopThread`): does nothing unless the player is
   active; otherwise marks the stop request `+0x43`, then by PSMF status: 0, 4, 0x100, 0x200 →
   `scePsmfPlayerStop` (on success sets the video result to "no data" `0x8061600C`, clears the
   audio remaining and got-frame state and sets stopping); 2 → `scePsmfPlayerReleasePsmf`;
   1 → `scePsmfPlayerDelete` and, on success, clears the active byte `+0x40` and the stopping
   state; any other status does nothing. */

void GfxMoviePlayerStopStep(GfxMoviePlayer *player)
{
  s32 status;

  if (player->active == 0) {
    return;
  }
  player->stopRequest = 1;
  status = scePsmfPlayerGetCurrentStatus(player->psmf);
  switch (status) {
  case 1:
    if (scePsmfPlayerDelete(player->psmf) != 0) {
      return;
    }
    player->active = 0;
    GfxMoviePlayerSetStopping(player, false);
    return;
  case 2:
    scePsmfPlayerReleasePsmf(player->psmf);
    return;
  case 0:
  case 4:
  case 0x100:
  case 0x200:
    if (scePsmfPlayerStop(player->psmf) == 0) {
      player->videoResult = (s32)0x8061600C;
      player->audioRemaining = 0;
      player->gotFrame = 0;
      player->stopping = 1;
    }
    return;
  default:
    return;
  }
}
