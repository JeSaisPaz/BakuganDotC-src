// bdc 0x08a29ea0 CollisionSegmentVsCapsuleFalse
#include "bdc.h"

/* Segment shape method for capsule queries (vtable `0x08af5564` entry 4): unsupported pair, always
   returns false. */
bool CollisionSegmentVsCapsuleFalse(void *seg, void *other, ScePspFVector4 *out)
{
    (void)seg;
    (void)other;
    (void)out;
    return false;
}
