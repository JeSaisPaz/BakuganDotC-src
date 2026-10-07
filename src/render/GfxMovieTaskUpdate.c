// bdc 0x089d6a04 GfxMovieTaskUpdate
#include "bdc.h"

/* Update of the movie task (id 10040): once the modules are loaded (`GfxMovieInitStep`) and the
   player exists (`GfxMovieHasPlayer`) steps `g_movieRequestState`: 1 → open movie
   `g_movieRequestId` (`GfxMoviePlayerRequestOpen`, alpha reset to 1.0), 2 → wait for the start
   (`GfxMoviePlayerIsStarted`), 3 → playing: when `GfxMoviePlayerTick` reports the end the id
   becomes -1; START or CROSS (pad bits 0x8/0x4000) skips the movie after 2 seconds of playback
   (pts at 90 kHz) unless its id is in `g_movieUnskippableIds`, fading out over 15 frames
   (`fadeCounter`, `alpha`) before stopping the player (`GfxMoviePlayerRequestStop`). Nothing is
   done while game thread 14 (the stop thread, `BootIsThreadRunning`) runs. */

void GfxMovieTaskUpdate(CoreTask *task)
{
  GfxMovieTask *self = (GfxMovieTask *)task;
  GfxMoviePlayer *player;
  u16 pressed;
  bool found;
  u32 i;

  if (!GfxMovieInitStep()) {
    return;
  }
  if (!GfxMovieHasPlayer()) {
    return;
  }
  switch (g_movieRequestState) {
  case 1:
    player = GfxMovieGetPlayer();
    if (GfxMoviePlayerRequestOpen(player, g_movieRequestId)) {
      g_movieRequestState = 2;
    }
    self->alpha = 1.0f;
    return;
  case 2:
    player = GfxMovieGetPlayer();
    if (GfxMoviePlayerIsStarted(player)) {
      g_movieRequestState = 3;
      g_movieRequestBusy = 0;
    }
    return;
  case 3:
    break;
  default:
    return;
  }

  if (g_movieRequestId < 0) {
    return;
  }
  player = GfxMovieGetPlayer();
  if (!GfxMoviePlayerTick(player)) {
    g_movieRequestId = -1;
    return;
  }
  if (self->skipping) {
    self->fadeCounter--;
    if (self->fadeCounter < 0 && BootIsThreadRunning(14) == 0) {
      player = GfxMovieGetPlayer();
      GfxMoviePlayerRequestStop(player);
    }
    self->alpha = (float)self->fadeCounter / 15.0f;
    return;
  }
  if (BootIsThreadRunning(14) != 0) {
    return;
  }
  pressed = 0;
  if (g_padState != NULL) {
    pressed = g_padState->pressed;
  }
  if ((pressed & 0x8) == 0 && (pressed & 0x4000) == 0) {
    return;
  }
  player = GfxMovieGetPlayer();
  if ((s32)player->pts * 30 / 90000 < 60) {
    return;
  }
  found = false;
  for (i = 0; i < 24; i++) {
    if (g_movieUnskippableIds[i] == g_movieRequestId) {
      found = true;
      break;
    }
  }
  if (found) {
    return;
  }
  self->skipping = 1;
  self->fadeCounter = 15;
}
