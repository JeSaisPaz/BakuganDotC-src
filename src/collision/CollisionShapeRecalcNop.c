// bdc 0x08a29eb8 CollisionShapeRecalcNop
#include "bdc.h"

/* Empty implementation of the collision shapes' "recalculate derived data" virtual (vtable entry 9,
   offset `+0x4c`) for the segment (`0x08af5564`) and box (`0x08af5684`) shapes, which have nothing
   to precompute. */
void CollisionShapeRecalcNop(void *shape)
{
    (void)shape;
}
