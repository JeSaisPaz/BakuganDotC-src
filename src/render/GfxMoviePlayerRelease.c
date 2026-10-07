// bdc 0x089d580c GfxMoviePlayerRelease
#include "bdc.h"

/* Tears down the movie (PSMF/MPEG) player state built by `GfxMoviePlayerInit`: frees the work
   buffers (`audioBuffer`, the ring buffer `data->buffer`), the two 0x88000-byte aligned frame
   buffers (`frameBuffers[1]`, `frameBuffers[0]`, `MemFreeAligned`), the strip table `vertices`
   and the small records `videoData`, `psmfInfo`, `playInfo`, `data`, `psmf`, then calls
   `sceMpegFinish`. With `flags & 1` the player itself is freed too. No-op for NULL. Called by
   `GfxMovieShutdown`. */

void GfxMoviePlayerRelease(GfxMoviePlayer *player, u32 flags)

{
  void *ptr;

  if (player == NULL) {
    return;
  }
  ptr = player->audioBuffer;
  if (ptr != NULL) {
    MemLock();
    MemFree(ptr, NULL, 0);
    MemUnlock();
    player->audioBuffer = NULL;
  }
  ptr = player->data->buffer;
  if (ptr != NULL) {
    MemLock();
    MemFree(ptr, NULL, 0);
    MemUnlock();
    player->data->buffer = NULL;
  }
  MemFreeAligned(player->frameBuffers[1]);
  MemFreeAligned(player->frameBuffers[0]);
  ptr = player->vertices;
  if (ptr != NULL) {
    MemLock();
    MemFree(ptr, NULL, 0);
    MemUnlock();
    player->vertices = NULL;
  }
  ptr = player->videoData;
  if (ptr != NULL) {
    MemLock();
    MemFree(ptr, NULL, 0);
    MemUnlock();
    player->videoData = NULL;
  }
  ptr = player->psmfInfo;
  if (ptr != NULL) {
    MemLock();
    MemFree(ptr, NULL, 0);
    MemUnlock();
    player->psmfInfo = NULL;
  }
  ptr = player->playInfo;
  if (ptr != NULL) {
    MemLock();
    MemFree(ptr, NULL, 0);
    MemUnlock();
    player->playInfo = NULL;
  }
  ptr = player->data;
  if (ptr != NULL) {
    MemLock();
    MemFree(ptr, NULL, 0);
    MemUnlock();
    player->data = NULL;
  }
  ptr = player->psmf;
  if (ptr != NULL) {
    MemLock();
    MemFree(ptr, NULL, 0);
    MemUnlock();
    player->psmf = NULL;
  }
  sceMpegFinish();
  if ((flags & 1) != 0) {
    MemLock();
    MemFree(player, NULL, 0);
    MemUnlock();
  }
}
