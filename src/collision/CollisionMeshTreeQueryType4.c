// bdc 0x089e4c64 CollisionMeshTreeQueryType4
#include "bdc.h"

/* Recursive bounding-volume-tree walk of one collision mesh part for swept-sphere queries (type 4
   of `CollisionMeshTestShape`, a `CollisionCapsule`): returns at once when
   `CollisionAabbOverlapCapsule` rejects the node box; inner nodes (`kind == 0`) recurse into
   both children; leaves test each listed face whose surface passes the mask (`-1` always passes)
   and whose plane the sweep start is not behind (`!(n.xyz . start - n.w < 0)`) with
   `CollisionTriangleTestSweptSphere`, then the optional `g_collisionFaceFilter`, and record the
   hit in `g_collisionHitResult` / `g_collisionHitInfo` when its distance beats `bestDist`
   (`!(bestDist <= d)`): `bestT` gets the sweep parameter `planeT`, and the query state's
   `sweepCenter` becomes `start + axis * planeT` (w = `radiusSq`); sets `g_collisionMeshHit`.
   The face filter (`g_collisionFaceFilter`, indirect call) gets no VFPU value. */

typedef bool (*CollisionFaceFilterFn)(const CollisionFacePart *part, const u16 *face);

void CollisionMeshTreeQueryType4(const CollisionFacePart *part, const CollisionBvhNode *node, const CollisionCapsule *query)
{
    float dist;
    s32 i;

    if (!CollisionAabbOverlapCapsule(node->aabb, query)) {
        return;
    }
    dist = g_collisionFloatInf;
    if (node->kind == 0) {
        CollisionMeshTreeQueryType4(part, (CollisionBvhNode *)PspPtr(node->left), query);
        CollisionMeshTreeQueryType4(part, (CollisionBvhNode *)PspPtr(node->right), query);
        return;
    }
    for (i = 0; i < node->kind; i++) {
        const u16 *face =
            (const u16 *)PspPtr(part->faces) + ((const u16 *)PspPtr(node->tri))[i] * 5;
        s32 surface = g_collisionMeshQuery.faceSurface[(s8)face[4] & 0xf];
        const ScePspFVector4 *plane;
        float planeT;

        if (surface != -1 && ((1 << surface) & g_collisionMeshQuery.surfaceMask) == 0) {
            continue;
        }
        plane = &((const ScePspFVector4 *)PspPtr(part->normals))[face[3]];
        /* vdot.t of the plane normal and the sweep start */
        if ((plane->x * query->start[0] + plane->y * query->start[1] + plane->z * query->start[2]) - plane->w < 0.0f) {
            continue;
        }
        if (!CollisionTriangleTestSweptSphere((void *)part, face, (void *)query, &dist)) {
            continue;
        }
        if (g_collisionFaceFilter != NULL &&
            !((CollisionFaceFilterFn)g_collisionFaceFilter)(part, face)) {
            continue;
        }
        if (g_collisionHitInfo.bestDist <= dist) {
            continue;
        }
        g_collisionHitInfo.bestDist = dist;
        g_collisionHitResult.point = g_collisionMeshQuery.point;
        g_collisionHitInfo.partIndex = g_collisionMeshQuery.partIndex;
        g_collisionHitInfo.part = (void *)part;
        g_collisionHitInfo.face = face;
        g_collisionHitInfo.surface = surface;
        g_collisionHitInfo.fromClosestPoint = g_collisionMeshQuery.closestOnEdge;
        planeT = g_collisionMeshQuery.planeT;
        g_collisionHitInfo.bestT = planeT;
        /* sweepCenter = start + axis * planeT (vmul.t/vadd.t); w keeps start's w lane (radiusSq) */
        g_collisionMeshQuery.sweepCenter.x = query->start[0] + query->axis[0] * planeT;
        g_collisionMeshQuery.sweepCenter.y = query->start[1] + query->axis[1] * planeT;
        g_collisionMeshQuery.sweepCenter.z = query->start[2] + query->axis[2] * planeT;
        g_collisionMeshQuery.sweepCenter.w = query->radiusSq;
        g_collisionMeshHit = 1;
    }
}
