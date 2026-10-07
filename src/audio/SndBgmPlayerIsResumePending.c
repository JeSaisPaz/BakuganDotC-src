// bdc 0x089c3810 SndBgmPlayerIsResumePending
#include "bdc.h"

/* Returns `resumePrevious` (+0x1b): a push-started track (`SndBgmPlayerPlayTrack` with
   `pushPrevious`) is still waiting to restore the previous track. */

u8 SndBgmPlayerIsResumePending(SndBgmPlayer *player)

{
  u8 value;
  
  CoreLockAcquire(player->lock);
  value = player->resumePrevious;
  CoreLockRelease(player->lock);
  return value;
}

