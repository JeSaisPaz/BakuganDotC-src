// bdc 0x089c3848 SndBgmPlayerRequestSuspend
#include "bdc.h"

/* Asks the player to quiesce for PSP suspend: under the lock sets `suspendRequested = 1`, clears
   `suspended` and `resumeRequested`, then wakes the player thread. The thread acknowledges in
   `SndBgmPlayerThreadStep` once the decoder is parked (and frees the volatile-memory file
   buffer). */

void SndBgmPlayerRequestSuspend(SndBgmPlayer *player)

{
  CoreLockAcquire(player->lock);
  player->suspended = '\0';
  player->suspendRequested = '\x01';
  player->resumeRequested = '\0';
  CoreLockRelease(player->lock);
  BootWakeupThread(player->channel + 9);
  return;
}

