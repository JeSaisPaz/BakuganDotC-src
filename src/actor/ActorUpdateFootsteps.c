// bdc 0x088de534 ActorUpdateFootsteps
#include "bdc.h"

/* While the actor walks (`state == 1`), reads the motion progress (`GfxModelGetMotionProgress`)
   and toggles the foot bit 0 of `footFlags` when the progress enters the step window: with the bit
   clear, progress in (0.4, 0.9) -- or in (`g_actorSlowStepMin`, `g_actorSlowStepMax`) =
   (0.2, 0.7) while `walkMotion` is the slow-walk slot 0x2e; with the bit set, progress above 0.9.
   After a toggle it calls `ActorFootstep` with foot 2 if the bit is now set, else 3. */

void ActorUpdateFootsteps(Actor *self)
{
    u32 flags;
    s32 stepped;
    s32 foot;
    float progress;

    progress = GfxModelGetMotionProgress(&self->base);
    if (self->state != 1) {
        return;
    }
    flags = self->footFlags;
    stepped = 0;
    if ((flags & 1) == 0) {
        if (self->walkMotion == 0x2e) {
            if (g_actorSlowStepMin < progress && !(g_actorSlowStepMax <= progress)) {
                stepped = 1;
                self->footFlags = flags ^ 1;
            }
        } else if (!(progress <= 0.4f) && progress < 0.9f) {
            stepped = 1;
            self->footFlags = flags ^ 1;
        }
    } else if (!(progress <= 0.9f)) {
        stepped = 1;
        self->footFlags = flags ^ 1;
    }
    if (stepped) {
        foot = 3;
        if ((self->footFlags & 1) != 0) {
            foot = 2;
        }
        ActorFootstep(self, foot);
    }
}
