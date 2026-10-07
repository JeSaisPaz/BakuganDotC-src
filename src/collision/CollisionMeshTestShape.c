// bdc 0x089e5bb4 CollisionMeshTestShape
#include "bdc.h"

/* Tests a query shape (`query`: prepared shape header, types 1..4) against a type-8 triangle-mesh
   collider block `meshBlock` (`CollisionShapeBlock`). Clears `g_collisionMeshHit`, stores the
   block's face-attribute surface table and `surfaceMask` in `g_collisionMeshQuery`, then by
   query type walks every mesh part (`parts[0..partCount)`, index in `partIndex`) with
   `CollisionMeshTreeQueryType1` .. `CollisionMeshTreeQueryType4`. A part with a rotation is
   tested against a copy of the query transformed into its local space (query vtable slot 6 with
   the part's `invWorldMatrix`) held in a lazily initialised static shape
   (`g_collisionMeshRayQuery`, `g_collisionMeshSegmentQuery`, `g_collisionMeshSphereQuery`,
   `g_collisionMeshCapsuleQuery`); ray and segment queries also clear
   `g_collisionHitInfo`.fromClosestPoint. Other query types test nothing.
   On a hit the normal of `g_collisionHitResult` is completed: the hit face's plane normal, or
   for a closest-point hit the normalised vector from the hit point to the sphere centre (sphere:
   the last part's query/static centre; capsule: g_collisionMeshQuery.sweepCenter), components
   clamped to [-1, 1], 0 when zero-length, w = 0; for a rotated part the normal is rotated and
   the point transformed (w = 1) by the part's rotation; the point is then copied to `hit`.
   Returns g_collisionMeshHit (non-zero on a hit). */

s32 CollisionMeshTestShape(void *meshBlock, void *query, float *hit, u32 surfaceMask)
{
    CollisionShapeBlock *block = meshBlock;
    const CollisionShapeBlock *shape = query;
    const VtblEntry *transform;
    const ScePspFVector4 *center;
    const ScePspFVector4 *planeNormal;
    const CollisionFacePart *part;
    const float *rotation;
    SegmentShape *capsuleSegment;
    const ScePspFMatrix4 *rot;
    ScePspFVector4 v;
    float k;
    s32 i;

    g_collisionMeshHit = 0;
    g_collisionMeshQuery.faceSurface = block->surfaceIds;
    center = (const ScePspFVector4 *)0;
    g_collisionMeshQuery.surfaceMask = surfaceMask;
    switch (shape->type) {
    case 1:
        if (g_collisionMeshRayQueryInit == 0) {
            g_collisionMeshRayQueryInit = 1;
            g_collisionMeshRayQuery.vtbl = g_collisionRayVtbl;
            g_collisionMeshRayQuery.type = 1;
        }
        g_collisionHitInfo.fromClosestPoint = 0;
        for (i = 0; i < block->partCount; i++) {
            g_collisionMeshQuery.partIndex = i;
            if (((const CollisionFacePart *)block->parts[i])->rotation != (const float *)0) {
                transform = &shape->vtbl[6];
                ((void (*)(const void *, const ScePspFMatrix4 *, void *))transform->fn)(
                    (const u8 *)shape + transform->delta,
                    &((const CollisionFacePart *)block->parts[i])->invWorldMatrix,
                    &g_collisionMeshRayQuery);
                part = block->parts[i];
                CollisionMeshTreeQueryType1(part, part->bvhRoot, &g_collisionMeshRayQuery);
            } else {
                part = block->parts[i];
                CollisionMeshTreeQueryType1(part, part->bvhRoot, query);
            }
        }
        break;
    case 2:
        if (g_collisionMeshSegmentQueryInit == 0) {
            g_collisionMeshSegmentQueryInit = 1;
            g_collisionMeshSegmentQuery.info = g_collisionSegmentVtbl;
            g_collisionMeshSegmentQuery.type = 2;
        }
        g_collisionHitInfo.fromClosestPoint = 0;
        for (i = 0; i < block->partCount; i++) {
            g_collisionMeshQuery.partIndex = i;
            if (((const CollisionFacePart *)block->parts[i])->rotation != (const float *)0) {
                transform = &shape->vtbl[6];
                ((void (*)(const void *, const ScePspFMatrix4 *, void *))transform->fn)(
                    (const u8 *)shape + transform->delta,
                    &((const CollisionFacePart *)block->parts[i])->invWorldMatrix,
                    &g_collisionMeshSegmentQuery);
                part = block->parts[i];
                /* The segment shape shares the ray shape's type/vtable/origin/dir prefix. */
                CollisionMeshTreeQueryType2(part, part->bvhRoot,
                                            (const CollisionRayShape *)&g_collisionMeshSegmentQuery);
            } else {
                part = block->parts[i];
                CollisionMeshTreeQueryType2(part, part->bvhRoot, query);
            }
        }
        break;
    case 3:
        if (g_collisionMeshSphereQueryInit == 0) {
            g_collisionMeshSphereQueryInit = 1;
            g_collisionMeshSphereQuery.vtbl = g_collisionSphereVtbl;
            g_collisionMeshSphereQuery.type = 3;
        }
        for (i = 0; i < block->partCount; i++) {
            g_collisionMeshQuery.partIndex = i;
            if (((const CollisionFacePart *)block->parts[i])->rotation != (const float *)0) {
                transform = &shape->vtbl[6];
                ((void (*)(const void *, const ScePspFMatrix4 *, void *))transform->fn)(
                    (const u8 *)shape + transform->delta,
                    &((const CollisionFacePart *)block->parts[i])->invWorldMatrix,
                    &g_collisionMeshSphereQuery);
                part = block->parts[i];
                CollisionMeshTreeQueryType3(part, part->bvhRoot, &g_collisionMeshSphereQuery);
                center = &g_collisionMeshSphereQuery.center;
            } else {
                part = block->parts[i];
                CollisionMeshTreeQueryType3(part, part->bvhRoot, query);
                center = &((const CollisionSphereQuery *)query)->center;
            }
        }
        break;
    case 4:
        if (g_collisionMeshCapsuleQueryInit == 0) {
            g_collisionMeshCapsuleQueryInit = 1;
            capsuleSegment = (SegmentShape *)g_collisionMeshCapsuleQuery.segmentHead;
            g_collisionMeshCapsuleQuery.vtbl = g_collisionCapsuleVtbl;
            capsuleSegment->info = g_collisionSegmentVtbl;
            capsuleSegment->type = 2;
            g_collisionMeshCapsuleQuery.type = 4;
        }
        for (i = 0; i < block->partCount; i++) {
            g_collisionMeshQuery.partIndex = i;
            if (((const CollisionFacePart *)block->parts[i])->rotation != (const float *)0) {
                transform = &shape->vtbl[6];
                ((void (*)(const void *, const ScePspFMatrix4 *, void *))transform->fn)(
                    (const u8 *)shape + transform->delta,
                    &((const CollisionFacePart *)block->parts[i])->invWorldMatrix,
                    &g_collisionMeshCapsuleQuery);
                part = block->parts[i];
                CollisionMeshTreeQueryType4(part, part->bvhRoot, &g_collisionMeshCapsuleQuery);
            } else {
                part = block->parts[i];
                CollisionMeshTreeQueryType4(part, part->bvhRoot, query);
            }
            center = &g_collisionMeshQuery.sweepCenter;
        }
        break;
    }

    if (g_collisionMeshHit != 0) {
        rotation = ((const CollisionFacePart *)g_collisionHitInfo.part)->rotation;
        if (rotation != (const float *)0) {
            rot = (const ScePspFMatrix4 *)rotation;
            if (g_collisionHitInfo.fromClosestPoint == 0) {
                part = g_collisionHitInfo.part;
                planeNormal = &part->normals[g_collisionHitInfo.face[3]];
                v = *planeNormal;
                /* vtfm3.t: xyz only; the sv.q also writes a stale VFPU lane to normal.w (left out). */
                g_collisionHitResult.normal.x = rot->x.x * v.x + rot->y.x * v.y + rot->z.x * v.z;
                g_collisionHitResult.normal.y = rot->x.y * v.x + rot->y.y * v.y + rot->z.y * v.z;
                g_collisionHitResult.normal.z = rot->x.z * v.x + rot->y.z * v.y + rot->z.z * v.z;
            } else {
                /* normal = clamp(normalize(rotation * (center - point))), w = 0 (S713). */
                v.x = center->x - g_collisionHitResult.point.x;
                v.y = center->y - g_collisionHitResult.point.y;
                v.z = center->z - g_collisionHitResult.point.z;
                g_collisionHitResult.normal.x = rot->x.x * v.x + rot->y.x * v.y + rot->z.x * v.z;
                g_collisionHitResult.normal.y = rot->x.y * v.x + rot->y.y * v.y + rot->z.y * v.z;
                g_collisionHitResult.normal.z = rot->x.z * v.x + rot->y.z * v.y + rot->z.z * v.z;
                v.x = g_collisionHitResult.normal.x;
                v.y = g_collisionHitResult.normal.y;
                v.z = g_collisionHitResult.normal.z;
                k = v.x * v.x + v.y * v.y + v.z * v.z;
                k = k == 0.0f ? 0.0f : VfRsq(k);
                g_collisionHitResult.normal.x = VfSat1(v.x * k);
                g_collisionHitResult.normal.y = VfSat1(v.y * k);
                g_collisionHitResult.normal.z = VfSat1(v.z * k);
                g_collisionHitResult.normal.w = 0.0f;
            }
            /* point = rotation * (point.xyz, 1). */
            v = g_collisionHitResult.point;
            g_collisionHitResult.point.x =
                rot->x.x * v.x + rot->y.x * v.y + rot->z.x * v.z + rot->w.x;
            g_collisionHitResult.point.y =
                rot->x.y * v.x + rot->y.y * v.y + rot->z.y * v.z + rot->w.y;
            g_collisionHitResult.point.z =
                rot->x.z * v.x + rot->y.z * v.y + rot->z.z * v.z + rot->w.z;
            g_collisionHitResult.point.w =
                rot->x.w * v.x + rot->y.w * v.y + rot->z.w * v.z + rot->w.w;
        } else if (g_collisionHitInfo.fromClosestPoint == 0) {
            part = g_collisionHitInfo.part;
            planeNormal = &part->normals[g_collisionHitInfo.face[3]];
            g_collisionHitResult.normal = *planeNormal;
        } else {
            /* normal = clamp(normalize(center - point)), w = 0 (S713). */
            v.x = center->x - g_collisionHitResult.point.x;
            v.y = center->y - g_collisionHitResult.point.y;
            v.z = center->z - g_collisionHitResult.point.z;
            k = v.x * v.x + v.y * v.y + v.z * v.z;
            k = k == 0.0f ? 0.0f : VfRsq(k);
            g_collisionHitResult.normal.x = VfSat1(v.x * k);
            g_collisionHitResult.normal.y = VfSat1(v.y * k);
            g_collisionHitResult.normal.z = VfSat1(v.z * k);
            g_collisionHitResult.normal.w = 0.0f;
        }
        hit[0] = g_collisionHitResult.point.x;
        hit[1] = g_collisionHitResult.point.y;
        hit[2] = g_collisionHitResult.point.z;
        hit[3] = g_collisionHitResult.point.w;
    }
    return g_collisionMeshHit;
}
