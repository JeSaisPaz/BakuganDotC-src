// bdc 0x089c3968 SndBgmPlayerStateIdle
#include "bdc.h"

/* State 0 handler of `SndBgmPlayer`'s state machine (entry 0 of `g_sndBgmPlayerStateTable`): if
   a stop command (3) is pending and the player was still marked playing, marks it `stopped` (and,
   on a channel other than 0, raises `decoderStop` so the decoder thread ends), then parks the
   player thread with `sceKernelSleepThreadCB` (helper `BootSleepCurrentThread`) until a request
   wakes it. */

void SndBgmPlayerStateIdle(SndBgmPlayer *player)

{
  CoreLockAcquire(player->lock);
  if (((player->command == 3) && (player->stopped == '\0')) &&
     (player->stopped = '\x01', player->channel != 0)) {
    player->decoderStop = '\x01';
  }
  CoreLockRelease(player->lock);
  BootSleepCurrentThread();
  return;
}

