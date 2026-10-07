// bdc 0x089c353c SndBgmPlayerPlayTrack
#include "bdc.h"

/* Starts playing `trackId` (file name from `SndGetStreamFilePath`) from scratch: accepted when
   the player is stopped (or `pushPrevious` is non-zero), its state is idle and the id differs from
   the current one. If a track is playing (`stopped == 0`) it is first faded out over 0.5 s
   (`SndBgmPlayerStopF`); `pushPrevious` is stored in `resumePrevious`. Returns 1 if accepted,
   else 0. An unknown `trackId` keeps the current track id and re-copies its path. */

s32 SndBgmPlayerPlayTrack(SndBgmPlayer *player, s32 trackId, u8 loop, u8 pushPrevious)
{
  s32 result = 0;
  s32 wake = 0;
  s32 current;
  char *src;

  CoreLockAcquire(player->lock);
  if ((player->stopped | pushPrevious) != 0) {
    current = player->trackId;
    if (current != trackId && player->state == 0) {
      if (player->prevTrackId != current && player->resumePrevious == 0) {
        player->prevTrackId = current;
      }
      src = SndGetStreamFilePath(trackId);
      if (src != NULL) {
        player->trackId = trackId;
      } else {
        src = SndGetStreamFilePath(player->trackId);
      }
      strncpy(player->path, src, sizeof(player->path));
      player->loopRequested = loop;
      if (player->stopped == 0) {
        SndBgmPlayerStopF(0.5f, player, 1);
      }
      wake = 1;
      player->state = 1;
      player->command = 2;
      player->stopped = 0;
      player->resumePrevious = pushPrevious;
      result = wake;
    }
  }
  CoreLockRelease(player->lock);
  if (wake) {
    BootWakeupThread(player->channel + 9);
  }
  return result;
}
