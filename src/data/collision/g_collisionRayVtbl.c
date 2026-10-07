// bdc 0x08af5504 g_collisionRayVtbl
#include "bdc.h"

__typeof__(VtblEntry[12]) g_collisionRayVtbl = {
    {0}, { .fn = (void *)CollisionRayVsRayFalse }, { .fn = (void *)CollisionRayVsSegmentFalse },
    { .fn = (void *)CollisionRayVsSphere }, { .fn = (void *)CollisionRayVsCapsule },
    { .fn = (void *)CollisionRayVsBox }, { .fn = (void *)CollisionRayTransform },
    { .fn = (void *)CollisionRayDebugDrawNop }, { .fn = (void *)CollisionRayGetCenter },
    { .fn = (void *)CollisionRayRecalc }, { .fn = (void *)CollisionShapeSetRadiusNop },
    { .fn = (void *)CollisionShapeGetRadiusZero },
};
