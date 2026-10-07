// bdc 0x089c37d8 SndBgmPlayerIsStopped
#include "bdc.h"

/* Returns the player's `stopped` flag (1 = nothing playing), read under the lock. */

u8 SndBgmPlayerIsStopped(SndBgmPlayer *player)

{
  u8 value;
  
  CoreLockAcquire(player->lock);
  value = player->stopped;
  CoreLockRelease(player->lock);
  return value;
}

