// bdc 0x089e4a5c CollisionTriangleTestSweptSphere
#include "bdc.h"

/* Tests one collision mesh triangle (`face` = three vertex indices into `part->vertices` and a
   plane index `face[3]` into `part->normals`) against a swept-sphere query (`CollisionCapsule`:
   `start`, `radiusSq`, `axis`, `axisLen`): `CollisionPlaneTestSweptSphere` gives the contact on
   the triangle's plane, `CollisionClosestPointOnTriangle` clamps it to the triangle (feature
   code in g_collisionMeshQuery.closestOnEdge, point in g_collisionMeshQuery.point), then the
   sphere/ray intersection along the reversed, normalised (VFPU `vrsq`) motion direction gives
   `*bestT = -b - sqrt(b*b - c)`. Returns 0 if the plane is missed, the start is outside the sphere
   and moving away (c > 0 and b > 0), or the discriminant is negative (`*bestT` untouched in those
   cases), or when `*bestT` is not below `axisLen` or the current g_collisionHitInfo.bestDist;
   otherwise stores the plane time in g_collisionMeshQuery.planeT and returns 1. */

s32 CollisionTriangleTestSweptSphere(void *part, const u16 *face, void *query, float *bestT)
{
    const CollisionFacePart *p = part;
    const CollisionCapsule *q = query;
    float planeT;
    ScePspFVector4 contact __attribute__((aligned(16)));
    float nx, ny, nz;
    float lenSq;
    float k;
    float dx, dy, dz;
    const ScePspFVector4 *verts;
    float b;
    float c;
    float disc;
    s32 hit;

    planeT = 0.0f;
    if (!CollisionPlaneTestSweptSphere((const float *)&((const ScePspFVector4 *)PspPtr(p->normals))[face[3]], query, &planeT,
                                       &contact.x)) {
        return 0;
    }
    verts = (const ScePspFVector4 *)PspPtr(p->vertices);
    g_collisionMeshQuery.closestOnEdge =
        CollisionClosestPointOnTriangle(&verts[face[0]].x, &verts[face[1]].x, &verts[face[2]].x,
                                        &contact.x, &g_collisionMeshQuery.point.x);

    /* dir = normalize(-axis): scale 1/sqrt(lenSq), or 0 (bank S713) for a zero-length axis; each
       lane saturated to [-1, 1] (vpfxd). diff = point - start; b = dot(diff, dir),
       c = dot(diff, diff). */
    nx = -q->axis[0];
    ny = -q->axis[1];
    nz = -q->axis[2];
    lenSq = nx * nx + ny * ny + nz * nz;
    k = VfRsq(lenSq);
    if (lenSq == 0.0f) {
        k = 0.0f;
    }
    nx = VfSat1(nx * k);
    ny = VfSat1(ny * k);
    nz = VfSat1(nz * k);
    dx = g_collisionMeshQuery.point.x - q->start[0];
    dy = g_collisionMeshQuery.point.y - q->start[1];
    dz = g_collisionMeshQuery.point.z - q->start[2];
    b = dx * nx + dy * ny + dz * nz;
    c = dx * dx + dy * dy + dz * dz;

    c = c - q->radiusSq;
    if (!(c <= 0.0f) && !(b <= 0.0f)) {
        hit = 0;
    } else {
        disc = b * b - c;
        if (disc < 0.0f) {
            hit = 0;
        } else {
            hit = 1;
            *bestT = -b - __builtin_sqrtf(disc);
        }
    }
    if (hit == 0) {
        return 0;
    }
    if (q->axisLen <= *bestT) {
        return 0;
    }
    if (g_collisionHitInfo.bestDist <= *bestT) {
        return 0;
    }
    g_collisionMeshQuery.planeT = planeT;
    return 1;
}
