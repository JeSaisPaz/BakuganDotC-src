// bdc 0x089c32d8 SndBgmPlayerSetVolume
#include "bdc.h"

/* Stores a pending volume change (`pendingVolume` percent 0..100, `pendingFadeMs`), sets
   `volumePending` and wakes the player thread, which forwards it to the decoder with
   `SndDecOutSetVolume` on its next `SndBgmPlayerThreadStep`. */

void SndBgmPlayerSetVolume(SndBgmPlayer *player, s32 percent, s32 fadeMs)

{
  CoreLockAcquire(player->lock);
  player->pendingVolume = percent;
  player->pendingFadeMs = fadeMs;
  player->volumePending = '\x01';
  CoreLockRelease(player->lock);
  BootWakeupThread(player->channel + 9);
  return;
}

