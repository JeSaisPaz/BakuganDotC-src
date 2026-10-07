// bdc 0x088e107c ActorPlayerScanOff
#include "bdc.h"

/* Switches off power B (scan, `scan`) when active and `force` or a cancel is pending: stops its
   loop sound `scanSound` and zeroes the colour of the screen effects 0xf/0x10. */

void ActorPlayerScanOff(ActorPlayer *self, u8 force)
{
  float color[4] __attribute__((aligned(16)));

  if ((self->scan != '\0') && ((force != '\0' || (self->powerRequest != '\0')))) {
    if (SndHasManager()) {
      SndManagerStop(SndGetManager(), self->scanSound);
    }
    self->scan = '\0';
    self->powerCooldown = 0;
    self->powerRequest = '\0';
    color[0] = 0.0f;
    color[1] = 0.0f;
    color[2] = 0.0f;
    color[3] = 0.0f;
    GfxEffectSetColorAttached(g_worldEffectMgr, 0xf, (float *)0x0, color);
    color[0] = 0.0f;
    color[1] = 0.0f;
    color[2] = 0.0f;
    color[3] = 0.0f;
    GfxEffectSetColorAttached(g_worldEffectMgr, 0x10, (float *)0x0, color);
  }
}
