// bdc 0x08a29eb0 CollisionRayDebugDrawNop
#include "bdc.h"

/* Debug-draw method of the ray shape (vtable `0x08af5504` entry 7, offset `+0x3c`): empty, rays are
   not drawn (compare `CollisionSegmentDebugDraw`). */
void CollisionRayDebugDrawNop(void *ray, const ScePspFVector4 *colour, const ScePspFMatrix4 *mtx)
{
    (void)ray;
    (void)colour;
    (void)mtx;
}
