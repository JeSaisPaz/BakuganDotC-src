// bdc 0x089e3c0c CollisionMeshTreeQueryType1
#include "bdc.h"

/* Recursive BVH walk of a collision mesh part for ray queries (type 1 of
   `CollisionMeshTestShape`): returns at once when the ray/box test `CollisionBvhTestRay` rejects
   the node box; inner nodes (`kind == 0`) recurse into both children; leaves test each listed face
   whose surface passes the mask (`-1` always passes) with `CollisionTriangleTestRay` and record
   the hit in `g_collisionHitResult` / `g_collisionHitInfo` when its ray parameter beats `bestT`
   (`!(bestT <= t)`), setting `g_collisionMeshHit`. */

void CollisionMeshTreeQueryType1(const CollisionFacePart *part, const CollisionBvhNode *node, const void *query)
{
    float t;
    s32 i;

    if (!CollisionBvhTestRay(node->aabb, query)) {
        return;
    }
    t = g_collisionFloatInf;
    if (node->kind == 0) {
        CollisionMeshTreeQueryType1(part, (CollisionBvhNode *)PspPtr(node->left), query);
        CollisionMeshTreeQueryType1(part, (CollisionBvhNode *)PspPtr(node->right), query);
        return;
    }
    for (i = 0; i < node->kind; i++) {
        const u16 *face =
            (const u16 *)PspPtr(part->faces) + ((const u16 *)PspPtr(node->tri))[i] * 5;
        s32 surface = g_collisionMeshQuery.faceSurface[(s8)face[4] & 0xf];

        if (surface != -1 && ((1 << surface) & g_collisionMeshQuery.surfaceMask) == 0) {
            continue;
        }
        if (!CollisionTriangleTestRay(part, face, query, &t)) {
            continue;
        }
        if (g_collisionHitInfo.bestT <= t) {
            continue;
        }
        g_collisionHitInfo.bestT = t;
        g_collisionHitResult.point = g_collisionMeshQuery.point;
        g_collisionHitInfo.partIndex = g_collisionMeshQuery.partIndex;
        g_collisionHitInfo.part = (void *)part;
        g_collisionHitInfo.face = face;
        g_collisionHitInfo.surface = surface;
        g_collisionMeshHit = 1;
    }
}
