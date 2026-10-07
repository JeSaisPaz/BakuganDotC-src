// bdc 0x089c3898 SndBgmPlayerRequestResume
#include "bdc.h"

/* Tells a suspended player that the PSP has resumed: sets `resumeRequested = 1` under the lock and
   wakes the thread, which clears `suspendRequested`, restarts the state machine from the unload
   state and refills the volatile buffer (`SndBgmPlayerThreadStep`). */

void SndBgmPlayerRequestResume(SndBgmPlayer *player)

{
  CoreLockAcquire(player->lock);
  player->resumeRequested = '\x01';
  CoreLockRelease(player->lock);
  BootWakeupThread(player->channel + 9);
  return;
}

