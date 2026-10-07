// bdc 0x08892ed8 BtlAiIsFrontBlocked
#include "bdc.h"

/* Casts three rays from the owner of a BtlAi — yaw (base.rot[1]) +45°, −45° and straight
   ahead — with BtlAiRaycastBlocked; returns 1 on the first blocked ray, else 0. Each ray passes
   a copy of the owner's position quad (taken once, before the loop) as `from` and the yaw
   direction (cos, 0, sin, 0) scaled by `dist` over x, y, z as `to` (a direction, not an end
   point). */
s32 BtlAiIsFrontBlocked(float dist, BtlAi *self)
{
    float pos[4];
    float from[4];
    float to[4];
    float yaw;
    s32 i;

    pos[0] = self->owner->base.pos[0];
    pos[1] = self->owner->base.pos[1];
    pos[2] = self->owner->base.pos[2];
    pos[3] = self->owner->base.pos[3];
    for (i = 0; i < 3; i++) {
        yaw = self->owner->base.rot[1];
        if (i == 0) {
            yaw = yaw + 0.785398185f; /* 0x3f490fdb, pi/4 */
        } else if (i == 1) {
            yaw = yaw - 0.785398185f;
        }
        /* vrot [C,0,S,0] of yaw * 2/pi (bank S703) then vscl.t by dist; lane w stays 0 */
        to[0] = __builtin_cosf(yaw) * dist;
        to[1] = 0.0f * dist;
        to[2] = __builtin_sinf(yaw) * dist;
        to[3] = 0.0f;
        from[0] = pos[0];
        from[1] = pos[1];
        from[2] = pos[2];
        from[3] = pos[3];
        if (BtlAiRaycastBlocked(self, from, to, NULL) != 0) {
            return 1;
        }
    }
    return 0;
}
