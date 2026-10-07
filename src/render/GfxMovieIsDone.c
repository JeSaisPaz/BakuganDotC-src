// bdc 0x089d67c8 GfxMovieIsDone
#include "bdc.h"

/* Returns whether the movie started by `GfxMovieRequestPlay` has finished (or there is no movie
   player): 1 when the movie player object `0x08ac5b80` does not exist; 0 while the busy/loading
   flag `0x08ac5b98` is set; otherwise the result of `GfxMoviePlayerIsStopped(player)`, which is 1
   when the player's playing byte (`+0x40`) is 0 and game thread slot 14 no longer exists
   (`BootGetThreadId(0xe) == -1`). Polled by `ScriptOpPlayMovie` and `ScriptOpControlMovie`
   before they kill the task with `CoreTaskRemove`. */

bool GfxMovieIsDone(void)
{
  if (!GfxMovieHasPlayer()) {
    return true;
  }
  if (g_movieRequestBusy != 0) {
    return false;
  }
  return GfxMoviePlayerIsStopped((GfxMoviePlayer *)GfxMovieGetPlayer());
}
