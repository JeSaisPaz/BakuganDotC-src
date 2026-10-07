// bdc 0x08a29e98 CollisionSegmentVsSphereFalse
#include "bdc.h"

/* Segment shape method for sphere queries (vtable `0x08af5564` entry 3): unsupported pair, always
   returns false. */

bool CollisionSegmentVsSphereFalse(void *seg, void *other, ScePspFVector4 *out)
{
    (void)seg;
    (void)other;
    (void)out;
    return false;
}
