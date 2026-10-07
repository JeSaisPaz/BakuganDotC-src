// bdc 0x08893010 BtlAiRaycastHeading
#include "bdc.h"

/* Casts a ray from the owner of `BtlAi` towards the point `dist` along world
   heading `heading` (relative vector {cos, 0, sin} * dist, passed as the `to` point) via
   `BtlAiRaycastBlocked` and returns its result (Ghidra shows `void`; the value passes
   through `v0`). */

s32 BtlAiRaycastHeading(float heading, float dist, BtlAi *self)
{
    float from[4];
    float to[4];
    BtlBakugan *owner;

    /* vmul.s by S703 (2/pi) then vrot.q [C,0,S,0]: cos/sin of `heading` in radians; vscl.t
       scales x, y, z by `dist` and leaves w at 0. The asm first stores C720 into the buffer,
       fully overwritten by this vector. */
    to[0] = __builtin_cosf(heading) * dist;
    to[1] = 0.0f * dist;
    to[2] = __builtin_sinf(heading) * dist;
    to[3] = 0.0f;
    owner = self->owner;
    from[0] = owner->base.pos[0];
    from[1] = owner->base.pos[1];
    from[2] = owner->base.pos[2];
    from[3] = owner->base.pos[3];
    return BtlAiRaycastBlocked(self, from, to, NULL);
}
