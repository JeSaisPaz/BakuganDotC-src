// bdc 0x089d5fb4 GfxMoviePlayerHasPendingRequest
#include "bdc.h"

/* Returns 1 when the movie player has an open request pending that has not started yet: request
   byte `+0x4c` set (`GfxMoviePlayerRequestOpen`) and started byte `+0x4d` clear
   (`GfxMoviePlayerIsStarted`); else 0. */

bool GfxMoviePlayerHasPendingRequest(GfxMoviePlayer *player)

{
  bool pending;

  pending = false;
  if ((player->openRequest != '\0') && (player->started == '\0')) {
    pending = true;
  }
  return pending;
}
