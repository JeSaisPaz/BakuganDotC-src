// bdc 0x089c32a0 SndBgmPlayerGetTrackId
#include "bdc.h"

/* Returns the id of the track currently selected on the player (`trackId`, -1 when none), read
   under the player lock. */

s32 SndBgmPlayerGetTrackId(SndBgmPlayer *player)

{
  s32 trackId;
  
  CoreLockAcquire(player->lock);
  trackId = player->trackId;
  CoreLockRelease(player->lock);
  return trackId;
}

