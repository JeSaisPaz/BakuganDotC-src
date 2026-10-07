// bdc 0x08af5564 g_collisionSegmentVtbl
#include "bdc.h"

__typeof__(VtblEntry[12]) g_collisionSegmentVtbl = {
    {0}, { .fn = (void *)CollisionRayVsRayFalse }, { .fn = (void *)CollisionRayVsSegmentFalse },
    { .fn = (void *)CollisionSegmentVsSphereFalse },
    { .fn = (void *)CollisionSegmentVsCapsuleFalse }, { .fn = (void *)CollisionSegmentVsBoxFalse },
    { .fn = (void *)CollisionSegmentTransform }, { .fn = (void *)CollisionSegmentDebugDraw },
    { .fn = (void *)CollisionSegmentGetCenter }, { .fn = (void *)CollisionShapeRecalcNop },
    { .fn = (void *)CollisionShapeSetRadiusNop }, { .fn = (void *)CollisionShapeGetRadiusZero },
};
