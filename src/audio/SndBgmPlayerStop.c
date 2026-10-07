// bdc 0x089c3678 SndBgmPlayerStop
#include "bdc.h"

/* Requests the player to stop: clears `resumePrevious`; unless a suspend or a stop is already
   pending, fades the decoder to 0 over `fadeMs` (`SndDecOutSetVolume`, only if a decoder exists
   and still has volume), then sets command 3 (stop), state 1, step 0 and — unless `keepPath` —
   clears the path buffer, and wakes the thread. Returns 0 only when the player is in state ≥ 2
   (loading/starting) or negative, else 1 (also when the request was ignored). */

s32 SndBgmPlayerStop(SndBgmPlayer *player, s32 fadeMs, u8 keepPath)
{
  s32 result = 1;
  s32 wake = 0;

  player->resumePrevious = 0;
  CoreLockAcquire(player->lock);
  if (player->state < 0) {
    result = 0;
  } else if (player->state >= 2) {
    result = 0;
  } else if (player->suspendRequested == 0 && player->command != 3) {
    if (SndDecOutExists(player->channel) != 0) {
      if (SndDecOutGetVolume(SndDecOutGet(player->channel)) != 0.0f) {
        SndDecOutSetVolume(SndDecOutGet(player->channel), 0, fadeMs);
      }
    }
    player->state = 1;
    player->command = 3;
    player->step = 0;
    if (keepPath == 0) {
      memset(player->path, 0, sizeof(player->path));
    }
    wake = 1;
  }
  CoreLockRelease(player->lock);
  if (wake) {
    BootWakeupThread(player->channel + 9);
  }
  return result;
}
