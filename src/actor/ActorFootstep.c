// bdc 0x088de4cc ActorFootstep
#include "bdc.h"

/* Footstep hook for one foot (`foot` 2 = left offset -20, 3 = right offset +20): unless the surface
   type is 4 (`ActorIsOnSurfaceType4`) computes the side offset perpendicular to the heading
   `rot[1]` (+0x34), but the result is unused in this build (the footprint/effect call was compiled out). */

void ActorFootstep(Actor *self, s32 foot)
{
    if (!ActorIsOnSurfaceType4(self)) {
        /* side offset (+-20 * perpendicular to rot[1] + pi/2) is computed into a dead stack vector */
        (void)foot;
    }
}
