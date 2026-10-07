// bdc 0x08a29ea8 CollisionSegmentVsBoxFalse
#include "bdc.h"

/* Segment shape method for box queries (vtable `0x08af5564` entry 5): unsupported pair, always
   returns false. */
bool CollisionSegmentVsBoxFalse(void *seg, void *other, ScePspFVector4 *out)
{
    (void)seg;
    (void)other;
    (void)out;
    return false;
}
