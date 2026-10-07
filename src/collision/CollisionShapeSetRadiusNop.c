// bdc 0x08a29ec0 CollisionShapeSetRadiusNop
#include "bdc.h"

/* Empty implementation of the collision shapes' set-radius virtual (entry 10, offset `+0x54`) for
   the ray (`0x08af5504`), segment (`0x08af5564`) and box (`0x08af5684`) shapes, which have no
   radius. */
void CollisionShapeSetRadiusNop(void *shape, float radius)
{
    (void)shape;
    (void)radius;
}
