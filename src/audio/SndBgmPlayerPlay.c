// bdc 0x089c349c SndBgmPlayerPlay
#include "bdc.h"

/* Starts playback of the file prepared by `SndBgmPlayerLoadTrack`: requires `loaded` and an idle
   state, stores `loop` (`loopRequested`), queues command 2 (play) with state 1 and clears
   `stopped`. Returns 1 when accepted, 0 otherwise; wakes the player thread. */

s32 SndBgmPlayerPlay(SndBgmPlayer *player, u8 loop)

{
  bool accepted;
  s32 result;
  
  result = 0;
  accepted = false;
  CoreLockAcquire(player->lock);
  if ((player->loaded != '\0') && (player->state == 0)) {
    accepted = true;
    player->loopRequested = loop;
    player->state = 1;
    player->command = 2;
    player->stopped = '\0';
    result = 1;
  }
  CoreLockRelease(player->lock);
  if (accepted) {
    BootWakeupThread(player->channel + 9);
  }
  return result;
}

