// bdc 0x08a29e90 CollisionRayVsSegmentFalse
#include "bdc.h"

/* Ray shape method for segment queries (vtable `0x08af5504` entry 2) (shared with the segment
   shape, vtable `0x08af5564`, which inherits it): unsupported pair, always returns false. */
bool CollisionRayVsSegmentFalse(void *ray, void *other, ScePspFVector4 *out)
{
    (void)ray;
    (void)other;
    (void)out;
    return false;
}
