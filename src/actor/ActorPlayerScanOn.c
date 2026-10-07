// bdc 0x088e117c ActorPlayerScanOn
#include "bdc.h"

/* Activates power B (scan) when requested (`powerRequest`) and neither power is active: plays
   sound `0x2c00026` and the loop `0x2c0002b` (handle kept in `scanSound`), sets `scan`, clears
   `powerCooldown` and the request, and colours the world effects 0xf/0x10 with `g_colorWhite`.
   While stealth is on it only plays the refusal sound 3. */

void ActorPlayerScanOn(ActorPlayer *self)
{
  if (self->stealth != 0) {
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 3, 0, 0);
    }
    return;
  }
  if (self->scan != 0 || self->powerRequest == 0) {
    return;
  }
  if (SndHasManager()) {
    SndManagerPlay(SndGetManager(), 0x2c00026, 0, 0);
  }
  if (SndHasManager()) {
    self->scanSound = SndManagerPlay(SndGetManager(), 0x2c0002b, 0, 0);
  }
  self->scan = 1;
  self->powerCooldown = 0;
  self->powerRequest = 0;
  GfxEffectSetColorAttached(g_worldEffectMgr, 0xf, NULL, (const float *)&g_colorWhite);
  GfxEffectSetColorAttached(g_worldEffectMgr, 0x10, NULL, (const float *)&g_colorWhite);
}
