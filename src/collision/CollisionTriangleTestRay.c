// bdc 0x089e3a20 CollisionTriangleTestRay
#include "bdc.h"

/* Ray/triangle test for query type 1: intersects the ray (query->origin along query->dir) with
   the face plane (normal part->normals[face[3]], plane distance in its w). Returns false unless
   the face is front-facing (dir . n < -0.0001), 0 <= t and t < *bestT. The hit point
   origin + t*dir (w = origin.w) is stored in g_collisionMeshQuery.point; returns false if it lies
   outside the triangle part->vertices[face[0..2]] (edge cross products disagree in sign),
   otherwise stores t in *bestT and returns true. */

/* a.xyz . b.xyz (vdot.t). */
static inline float Vec3Dot(const ScePspFVector4 *a, const ScePspFVector4 *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

/* out.xyz = a.xyz - b.xyz (vsub.t); w is never read. */
static inline void Vec3Sub(ScePspFVector4 *out, const ScePspFVector4 *a, const ScePspFVector4 *b)
{
    out->x = a->x - b->x;
    out->y = a->y - b->y;
    out->z = a->z - b->z;
    out->w = a->w;
}

/* out.xyz = a.xyz x b.xyz (vcrsp.t); the asm stores S713 (0.0f) in w, never read. */
static inline void Vec3Cross(ScePspFVector4 *out, const ScePspFVector4 *a, const ScePspFVector4 *b)
{
    out->x = a->y * b->z - a->z * b->y;
    out->y = a->z * b->x - a->x * b->z;
    out->z = a->x * b->y - a->y * b->x;
    out->w = 0.0f;
}

bool CollisionTriangleTestRay(const CollisionFacePart *part, const u16 *face, const CollisionRayShape *query, float *bestT)
{
    const ScePspFVector4 *normal = &((const ScePspFVector4 *)PspPtr(part->normals))[face[3]];
    ScePspFVector4 *point = &g_collisionMeshQuery.point;
    ScePspFVector4 e2;
    ScePspFVector4 e1;
    ScePspFVector4 e0;
    ScePspFVector4 c0;
    ScePspFVector4 c1;
    ScePspFVector4 c2;
    float denom;
    float t;

    denom = Vec3Dot(normal, &query->dir);
    if (!(denom < -0.0001f))
        return false;
    t = (normal->w - Vec3Dot(normal, &query->origin)) / denom;
    if (t < 0.0f)
        return false;
    if (!(t < *bestT))
        return false;

    /* point = origin + dir * t (vmul.t then vadd.t; w = origin.w) */
    point->x = query->origin.x + query->dir.x * t;
    point->y = query->origin.y + query->dir.y * t;
    point->z = query->origin.z + query->dir.z * t;
    point->w = query->origin.w;

    Vec3Sub(&e2, &((const ScePspFVector4 *)PspPtr(part->vertices))[face[2]], point);
    Vec3Sub(&e1, &((const ScePspFVector4 *)PspPtr(part->vertices))[face[1]], point);
    Vec3Sub(&e0, &((const ScePspFVector4 *)PspPtr(part->vertices))[face[0]], point);
    Vec3Cross(&c0, &e1, &e0);
    Vec3Cross(&c1, &e0, &e2);
    if (Vec3Dot(&c0, &c1) < 0.0f)
        return false;
    Vec3Cross(&c2, &e2, &e1);
    if (Vec3Dot(&c0, &c2) < 0.0f)
        return false;
    *bestT = t;
    return true;
}
