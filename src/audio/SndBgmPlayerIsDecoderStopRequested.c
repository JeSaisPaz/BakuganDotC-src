// bdc 0x089c38e0 SndBgmPlayerIsDecoderStopRequested
#include "bdc.h"

/* Returns the player's `decoderStop` flag (+0xc9), read under the lock. `SndDecOutThreadStep`
   polls it and, when set, marks its decoder `finished` so the decoder thread exits. */

u8 SndBgmPlayerIsDecoderStopRequested(SndBgmPlayer *player)

{
  u8 value;
  
  CoreLockAcquire(player->lock);
  value = player->decoderStop;
  CoreLockRelease(player->lock);
  return value;
}

