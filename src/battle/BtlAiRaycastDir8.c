// bdc 0x08893080 BtlAiRaycastDir8
#include "bdc.h"

/* Casts a ray of length `dist` from the owner of `BtlAi` via `BtlAiRaycastHeading`
   along the owner's yaw turned by the 16-bit direction `dir`: 0x2000/0x4000/0x6000 subtract
   45°/90°/135°, 0xa000/0xc000/0xe000 add 135°/90°/45°, 0x8000 adds 180° (behind); 0 and any other
   value keep the yaw. Returns the ray result (non-zero when blocked). Used to check room before
   sidesteps and dashes. */

s32 BtlAiRaycastDir8(float dist, BtlAi *self, s32 dir)
{
    float heading;

    heading = self->owner->base.rot[1];
    switch (dir) {
    case 0x2000:
        heading = heading - 0.785398185f; /* 0x3f490fdb */
        break;
    case 0x4000:
        heading = heading - 1.57079637f; /* 0x3fc90fdb */
        break;
    case 0x6000:
        heading = heading - 2.3561945f; /* 0x4016cbe4 */
        break;
    case 0x8000:
        heading = heading + 3.14159274f; /* 0x40490fdb */
        break;
    case 0xa000:
        heading = heading + 2.3561945f;
        break;
    case 0xc000:
        heading = heading + 1.57079637f;
        break;
    case 0xe000:
        heading = heading + 0.785398185f;
        break;
    default:
        break;
    }
    return BtlAiRaycastHeading(heading, dist, self);
}
