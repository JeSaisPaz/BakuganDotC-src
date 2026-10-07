// bdc 0x089e4634 CollisionMeshTreeQueryType3
#include "bdc.h"

/* Recursive bounding-volume-tree walk of one collision mesh part for sphere queries (type 3 of
   `CollisionMeshTestShape`): returns at once when `CollisionAabbOverlapSphere` rejects the node
   box; inner nodes (`kind == 0`) recurse into both children; leaves test each listed face whose
   surface passes the mask (`-1` always passes) and whose plane the sphere centre is not behind
   (`!(n.xyz . c - n.w < 0)`) with `CollisionTriangleTestSphere`, then the optional
   `g_collisionFaceFilter`, and record the hit in `g_collisionHitResult` / `g_collisionHitInfo`
   when its squared distance beats `bestDist` (`!(bestDist <= d)`), setting `g_collisionMeshHit`. */

typedef bool (*CollisionFaceFilterFn)(const CollisionFacePart *part, const u16 *face);

void CollisionMeshTreeQueryType3(const CollisionFacePart *part, const CollisionBvhNode *node, const CollisionSphereQuery *query)
{
    float distSq;
    s32 i;

    if (!CollisionAabbOverlapSphere(node->aabb, query)) {
        return;
    }
    distSq = g_collisionFloatInf;
    if (node->kind == 0) {
        CollisionMeshTreeQueryType3(part, node->left, query);
        CollisionMeshTreeQueryType3(part, node->right, query);
        return;
    }
    for (i = 0; i < node->kind; i++) {
        const u16 *face = part->faces + node->triList[i] * 5;
        s32 surface = g_collisionMeshQuery.faceSurface[(s8)face[4] & 0xf];
        const ScePspFVector4 *plane;

        if (surface != -1 && ((1 << surface) & g_collisionMeshQuery.surfaceMask) == 0) {
            continue;
        }
        plane = &part->normals[face[3]];
        /* vdot.t of the plane normal and the sphere centre (VFPU registers dead afterwards) */
        if ((plane->x * query->center.x + plane->y * query->center.y + plane->z * query->center.z) - plane->w < 0.0f) {
            continue;
        }
        if (!CollisionTriangleTestSphere(&part->vertices[face[0]].x, &part->vertices[face[1]].x,
                                         &part->vertices[face[2]].x, query, &distSq)) {
            continue;
        }
        if (g_collisionFaceFilter != NULL &&
            !((CollisionFaceFilterFn)g_collisionFaceFilter)(part, face)) {
            continue;
        }
        if (g_collisionHitInfo.bestDist <= distSq) {
            continue;
        }
        g_collisionHitInfo.bestDist = distSq;
        g_collisionHitResult.point = g_collisionMeshQuery.point;
        g_collisionHitInfo.partIndex = g_collisionMeshQuery.partIndex;
        g_collisionHitInfo.part = (void *)part;
        g_collisionHitInfo.face = face;
        g_collisionHitInfo.surface = surface;
        g_collisionHitInfo.fromClosestPoint = g_collisionMeshQuery.closestOnEdge;
        g_collisionMeshHit = 1;
    }
}
