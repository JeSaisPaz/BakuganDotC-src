// bdc 0x089d5c84 GfxMoviePlayerFetchVideo
#include "bdc.h"

/* Does nothing unless the player is active. At PSMF status 4 (playing), unless stopping or no
   frame is ready (`+0x42`), flips between the two frame buffers (`frameIndex` into
   `frameBuffers`), calls `scePsmfPlayerGetVideoData` and stores the result in `videoResult`; on
   success the first frame sets `firstFrame`, later ones set `gotFrame`. Errors other than "no
   data" `0x8061600C` count in `g_movieVideoErrorCount`. At end of stream (status 0x200), or at
   any other status when the stop request `+0x43` is set, it requests the stop
   (`GfxMoviePlayerRequestStop`) and clears `frameReady` if that succeeded. */

void GfxMoviePlayerFetchVideo(GfxMoviePlayer *player)
{
  s32 status;
  s32 result;

  if (GfxMoviePlayerIsActive(player) == 0) {
    return;
  }
  status = scePsmfPlayerGetCurrentStatus(player->psmf);
  if (status == 4) {
    if (GfxMoviePlayerIsStopping(player)) {
      return;
    }
    if (player->frameReady == 0) {
      return;
    }
    player->frameIndex = player->frameIndex + 1;
    if (!(player->frameIndex < 2)) {
      player->frameIndex = 0;
    }
    player->videoData->displaybuf = player->frameBuffers[player->frameIndex];
    result = scePsmfPlayerGetVideoData(player->psmf, player->videoData);
    player->videoResult = result;
    if (result == 0) {
      if (player->firstFrame == 0) {
        player->firstFrame = player->firstFrame + 1;
        return;
      }
      player->gotFrame = 1;
      return;
    }
    if (result == (s32)0x8061600C) {
      return;
    }
    g_movieVideoErrorCount = g_movieVideoErrorCount + 1;
    return;
  }
  if (status != 0x200 && player->stopRequest == 0) {
    return;
  }
  if (GfxMoviePlayerRequestStop(player)) {
    player->frameReady = 0;
  }
}
