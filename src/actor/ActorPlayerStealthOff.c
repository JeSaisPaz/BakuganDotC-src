// bdc 0x088e0fc4 ActorPlayerStealthOff
#include "bdc.h"

/* Switches off power A (stealth, `stealth`) when it is active and `force` or a cancel is pending
   (`powerRequest`): sound `0x2c00028`, burst effect 0x44 and stop of the loop effect 10 on the
   anchor `mtx + 0xc`, cooldown `powerCooldown = -15`. */

void ActorPlayerStealthOff(ActorPlayer *self, u8 force)
{
  float *attach;

  if ((self->stealth != '\0') && ((force != '\0' || (self->powerRequest != '\0')))) {
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0x2c00028, 0, 0);
    }
    attach = (self->base).mtx + 0xc;
    GfxEffectSpawnAttached(g_worldEffectMgr, 0x44, attach);
    GfxEffectStopAttached(g_worldEffectMgr, 10, attach);
    self->stealth = '\0';
    self->powerCooldown = -0xf;
    self->powerRequest = '\0';
  }
}
