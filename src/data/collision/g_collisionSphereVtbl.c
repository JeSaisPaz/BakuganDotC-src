// bdc 0x08af55c4 g_collisionSphereVtbl
#include "bdc.h"

__typeof__(VtblEntry[12]) g_collisionSphereVtbl = {
    {0}, { .fn = (void *)CollisionSphereVsRay }, { .fn = (void *)CollisionSphereVsSegment },
    { .fn = (void *)CollisionSphereVsSphere }, { .fn = (void *)CollisionSphereVsCapsule },
    { .fn = (void *)CollisionSphereVsBox }, { .fn = (void *)CollisionSphereTransform },
    { .fn = (void *)CollisionSphereDebugDraw }, { .fn = (void *)CollisionSphereGetCenter },
    { .fn = (void *)CollisionSphereRecalc }, { .fn = (void *)CollisionSphereSetRadius },
    { .fn = (void *)CollisionSphereGetRadius },
};
