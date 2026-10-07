// bdc 0x088e565c ActorNpcCloakUpdateFlicker
#include "bdc.h"

/* Flicker animation of the cloaked guard in mode 3 (does nothing in any other mode): forces the
   shadow on, then by `flickerPhase`: 0 fades the alpha (`ambient[3]`) up to 0.2 with a sine over
   90 degrees (step 15 per frame), 1 holds 0.2 for 30 frames, 2 fades out with a cosine and returns
   to phase 1 and mode 0 (hidden, `ActorNpcCloakSetMode`). Phases outside 0..2 do nothing.
   The sine/cosine are vsin.s/vcos.s of the angle in radians times the bank's 2/pi (S703), i.e. the
   plain sine/cosine of the angle. */

void ActorNpcCloakUpdateFlicker(ActorNpcCloak *self)
{
    s32 phase;
    s32 frame;

    if (self->mode != 3) {
        return;
    }
    ((BtlShadow *)self->base.base.base.shadow)->enabled = 1;
    phase = self->flickerPhase;
    if (phase < 0) {
        return;
    }
    if (phase == 0) {
        frame = self->flickerFrame;
        self->base.base.base.base.ambient[3] =
            __builtin_sinf((float)frame * 0.0055555557f * 3.1415927f) * 0.2f;
        if (frame >= 0x5b) {
            self->flickerPhase = 1;
            self->flickerFrame = 0;
        }
        self->flickerFrame = self->flickerFrame + 15;
    } else if (phase == 1) {
        frame = self->flickerFrame;
        self->base.base.base.base.ambient[3] = 0.2f;
        frame = frame + 1;
        self->flickerFrame = frame;
        if (frame >= 0x1f) {
            self->flickerPhase = 2;
            self->flickerFrame = 0;
        }
    } else if (phase == 2) {
        frame = self->flickerFrame;
        self->base.base.base.base.ambient[3] =
            __builtin_cosf((float)frame * 0.0055555557f * 3.1415927f) * 0.2f;
        frame = frame + 15;
        self->flickerFrame = frame;
        if (frame >= 0x5b) {
            self->flickerPhase = 1;
            self->flickerFrame = 0;
            ActorNpcCloakSetMode(self, 0);
        }
    }
}
