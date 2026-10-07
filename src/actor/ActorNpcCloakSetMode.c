// bdc 0x088e53d4 ActorNpcCloakSetMode
#include "bdc.h"

/* Sets the visibility mode `+0x460` of the cloaked guard class (model 0x4f, `ActorNpcCloakCtor`,
   vtable `0x08af39e4`): 0 hidden, 1 alerted, 2 revealed (opaque colour, virtual slot 3 with 1), 3
   flicker; other modes call slot 3 with 0. Unless the NPC is in state 6, also toggles the
   visibility byte `+0x25` of its view-cone object `+0x418` (1 while visible). The colour is
   (1, 1, 1, 1) packed to bytes (scale 255). */

void ActorNpcCloakSetMode(ActorNpcCloak *self, s32 mode)
{
    const VtblEntry *slot3;
    u32 lane;

    self->mode = mode;
    slot3 = &((const VtblEntry *)self->base.base.base.base.base.vtable)[3];
    if (mode == 2) {
        /* colour (1, 1, 1, 1) packed to RGBA8: all four lanes are the same */
        lane = VfI2uc(VfF2iz(VfSat0(1.0f) * 255.0f, 23));
        self->base.base.base.base.fogColor = lane | lane << 8 | lane << 16 | lane << 24;
        ((void (*)(void *, s32))slot3->fn)((u8 *)self + slot3->delta, 1);
    } else {
        ((void (*)(void *, s32))slot3->fn)((u8 *)self + slot3->delta, 0);
    }
    if (self->base.base.aiState != 6) {
        if (self->mode == 0) {
            ((ActorNpcViewCone *)self->base.base.viewCone)->visible = 0;
        } else {
            ((ActorNpcViewCone *)self->base.base.viewCone)->visible = 1;
        }
    }
}
