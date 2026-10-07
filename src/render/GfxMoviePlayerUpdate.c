// bdc 0x089d5a10 GfxMoviePlayerUpdate
#include "bdc.h"

/* Per-tick PSMF update: clears the frame-ready byte `+0x42`; while the player is active and not
   stopping, and `scePsmfPlayerGetCurrentStatus` is 4 (playing), calls `scePsmfPlayerUpdate` and
   sets `+0x42` on success, then keeps the console awake (`sceKernelPowerTick(0)`). */

void GfxMoviePlayerUpdate(GfxMoviePlayer *player)

{
  s32 status;

  player->frameReady = '\0';
  if (GfxMoviePlayerIsActive(player) != 0 && !GfxMoviePlayerIsStopping(player)) {
    status = scePsmfPlayerGetCurrentStatus(player->psmf);
    if ((status == 4) && (status = scePsmfPlayerUpdate(player->psmf), status == 0)) {
      player->frameReady = '\x01';
    }
    sceKernelPowerTick(0);
  }
  return;
}

