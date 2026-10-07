// bdc 0x089d6760 GfxMovieRequestPlay
#include "bdc.h"

/* Requests playback of movie `movieId`: if the movie player object (`0x08ac5b80`) exists it stores
   `movieId` in `0x08ac5b94`, sets the busy flag `0x08ac5b98` and the movie-task state `0x08ac5b9c`
   to 1 and starts game thread slot 13 with `BootStartThread``(0xd, NULL, 0)`, then returns true;
   returns false when there is no movie player. Polled by `ScriptOpPlayMovie` (returns 0 = done
   once it succeeds) and `ScriptOpControlMovie`. */

bool GfxMovieRequestPlay(u32 movieId)

{
  bool ok;

  ok = false;
  if (GfxMovieHasPlayer()) {
    g_movieRequestId = movieId;
    ok = true;
    g_movieRequestState = 1;
    g_movieRequestBusy = 1;
    BootStartThread(0xd,(void *)0x0,0);
  }
  return ok;
}

