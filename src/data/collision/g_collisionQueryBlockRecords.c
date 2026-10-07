// bdc 0x08ac5cec g_collisionQueryBlockRecords
#include "bdc.h"

__typeof__(CxxGlobalRecord[10]) g_collisionQueryBlockRecords = {
    {
        .object = (void *)&g_collisionRayBlock,
        .destructor = (void (*)(void *))CollisionShapeBlockDtor,
    },
    {
        .object = (void *)&g_collisionRayBlock2,
        .destructor = (void (*)(void *))CollisionShapeBlockDtor,
    },
    {
        .object = (void *)&g_collisionSegmentBlock,
        .destructor = (void (*)(void *))CollisionShapeBlockDtor,
    },
    {
        .object = (void *)&g_collisionSegmentBlock2,
        .destructor = (void (*)(void *))CollisionShapeBlockDtor,
    },
    {
        .object = (void *)&g_collisionSphereBlock,
        .destructor = (void (*)(void *))CollisionShapeBlockDtor,
    },
    {
        .object = (void *)&g_collisionSphereBlock2,
        .destructor = (void (*)(void *))CollisionShapeBlockDtor,
    },
    {
        .object = (void *)&g_collisionSweptSphereDesc.shapeBlock,
        .destructor = (void (*)(void *))CollisionShapeBlockDtor,
    },
    {
        .object = (void *)&g_collisionSweptSphereBlock2,
        .destructor = (void (*)(void *))CollisionShapeBlockDtor,
    },
    {
        .object = (void *)&g_collisionBoxBlock,
        .destructor = (void (*)(void *))CollisionShapeBlockDtor,
    },
    {
        .object = (void *)&g_collisionBoxBlock2,
        .destructor = (void (*)(void *))CollisionShapeBlockDtor,
    },
};
