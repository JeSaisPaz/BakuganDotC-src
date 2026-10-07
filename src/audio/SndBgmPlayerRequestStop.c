// bdc 0x089c3918 SndBgmPlayerRequestStop
#include "bdc.h"

/* Internal stop request issued by the decoder when its stream has run out: under the lock sets
   `command = 3` (stop), `state = 1` (unload), `step = 0`, then wakes the player thread. */

void SndBgmPlayerRequestStop(SndBgmPlayer *player)

{
  CoreLockAcquire(player->lock);
  player->command = 3;
  player->state = 1;
  player->step = 0;
  CoreLockRelease(player->lock);
  BootWakeupThread(player->channel + 9);
  return;
}

