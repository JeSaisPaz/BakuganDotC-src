// bdc 0x089c6ff0 SndManagerStateRun
#include "bdc.h"

/* State 5 (steady state) of the sound manager (`SndManagerStep`), executed once per vblank: under
   the manager lock it runs `SndManagerProcessCommands` (drains the command queue, updates group
   slots and voices); then it switches the output mode of decoder output 0 while a movie plays: if
   the movie/video object `g_moviePlayer` exists and its playing byte (`+0x40`,
   `GfxMoviePlayerIsActive`) is set it puts decoder output 0 into mode 6 (`SndDecOutSetMode`),
   otherwise it restores mode 0 if the mode is currently 6 (`SndDecOutGetMode`). */

void SndManagerStateRun(SndManager *mgr)

{
  bool moviePlaying;
  GfxMoviePlayer *player;
  SndDecOut *dec;

  moviePlaying = false;
  CoreLockAcquire(mgr->lock);
  SndManagerProcessCommands(mgr);
  CoreLockRelease(mgr->lock);
  if (GfxMovieHasPlayer()) {
    player = GfxMovieGetPlayer();
    if (GfxMoviePlayerIsActive(player)) {
      moviePlaying = true;
    }
  }
  if (SndDecOutExists(0) != 0) {
    if (moviePlaying) {
      dec = SndDecOutGet(0);
      if (SndDecOutGetMode(dec) != 6) {
        dec = SndDecOutGet(0);
        SndDecOutSetMode(dec, 6);
      }
    }
    else {
      dec = SndDecOutGet(0);
      if (SndDecOutGetMode(dec) == 6) {
        dec = SndDecOutGet(0);
        SndDecOutSetMode(dec, 0);
      }
    }
  }
  return;
}
