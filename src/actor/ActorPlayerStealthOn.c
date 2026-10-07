// bdc 0x088e12a0 ActorPlayerStealthOn
#include "bdc.h"

/* Activates power A (stealth) when requested (`powerRequest`) and neither power is active: sounds
   `0x2c00029`/`0x2c0002a`, effects 0x26 and 10 attached to the translation row of `base.mtx`,
   `stealth = 1`, `powerCooldown = -15`, clears `powerRequest`. While scan is on it only plays the
   refusal sound 3. */

void ActorPlayerStealthOn(ActorPlayer *self)
{
    float *attach;

    if (self->scan != 0) {
        if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 3, 0, 0);
        }
        return;
    }
    if (self->stealth != 0 || self->powerRequest == 0) {
        return;
    }
    if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0x2c00029, 0, 0);
    }
    if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0x2c0002a, 0, 0);
    }
    attach = &self->base.mtx[12];
    GfxEffectSpawnAttached(g_worldEffectMgr, 0x26, attach);
    GfxEffectSpawnAttached(g_worldEffectMgr, 10, attach);
    self->stealth = 1;
    self->powerCooldown = -15;
    self->powerRequest = 0;
}
