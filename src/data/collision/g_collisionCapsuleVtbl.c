// bdc 0x08af5624 g_collisionCapsuleVtbl
#include "bdc.h"

__typeof__(VtblEntry[12]) g_collisionCapsuleVtbl = {
    {0}, { .fn = (void *)CollisionCapsuleVsRay }, { .fn = (void *)CollisionCapsuleVsSegment },
    { .fn = (void *)CollisionCapsuleVsSphere }, { .fn = (void *)CollisionCapsuleVsCapsule },
    { .fn = (void *)CollisionCapsuleVsBox }, { .fn = (void *)CollisionCapsuleTransform },
    { .fn = (void *)CollisionCapsuleDebugDraw }, { .fn = (void *)CollisionCapsuleGetCenter },
    { .fn = (void *)CollisionCapsuleRecalc }, { .fn = (void *)CollisionCapsuleSetRadius },
    { .fn = (void *)CollisionCapsuleGetRadius },
};
