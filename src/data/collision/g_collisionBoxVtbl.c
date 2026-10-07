// bdc 0x08af5684 g_collisionBoxVtbl
#include "bdc.h"

__typeof__(VtblEntry[12]) g_collisionBoxVtbl = {
    {0}, { .fn = (void *)CollisionBoxVsRay }, { .fn = (void *)CollisionBoxVsSegment },
    { .fn = (void *)CollisionBoxVsSphere }, { .fn = (void *)CollisionBoxVsCapsule },
    { .fn = (void *)CollisionBoxVsBoxFalse }, { .fn = (void *)CollisionBoxTransform },
    { .fn = (void *)CollisionBoxDebugDraw }, { .fn = (void *)CollisionBoxGetCenter },
    { .fn = (void *)CollisionShapeRecalcNop }, { .fn = (void *)CollisionShapeSetRadiusNop },
    { .fn = (void *)CollisionShapeGetRadiusZero },
};
