// bdc 0x089d5f74 GfxMoviePlayerRequestOpen
#include "bdc.h"

/* Requests movie `movieId` (0..0x77): when no request or start is pending (`+0x4c`, `+0x4d`) sets
   the request byte `+0x4c` and stores the id in `+0x50`; returns 1 when accepted. State 1 of
   `GfxMovieTaskUpdate`. */

bool GfxMoviePlayerRequestOpen(GfxMoviePlayer *player, s32 movieId)

{
  bool accepted;

  accepted = false;
  if ((((-1 < movieId) && ((uint)movieId < 0x78)) && (player->openRequest == '\0')) &&
     (player->started == '\0')) {
    player->openRequest = '\x01';
    player->movieId = movieId;
    accepted = true;
  }
  return accepted;
}
