// bdc 0x089e45a0 CollisionTriangleTestSphere
#include "bdc.h"

/* Sphere/triangle test for query type 3: closest point of the triangle `abc` to the sphere centre
   `query->center` (w = squared radius, set by `CollisionSphereRecalc`) via
   `CollisionClosestPointOnTriangle` (feature code into g_collisionMeshQuery.closestOnEdge, point
   into g_collisionMeshQuery.point), stores the squared distance in `*distSq` and returns whether it
   is within the squared radius (true when !(radiusSq < distSq)). */

bool CollisionTriangleTestSphere(const float *a, const float *b, const float *c, const CollisionSphereQuery *query, float *distSq)
{
    const ScePspFVector4 *center = &query->center;
    float dx, dy, dz, d;

    g_collisionMeshQuery.closestOnEdge = CollisionClosestPointOnTriangle(a, b, c, &center->x, &g_collisionMeshQuery.point.x);
    dx = VfSub(g_collisionMeshQuery.point.x, center->x);
    dy = VfSub(g_collisionMeshQuery.point.y, center->y);
    dz = VfSub(g_collisionMeshQuery.point.z, center->z);
    d = VfDot3(dx, dy, dz, dx, dy, dz);
    *distSq = d;
    return !(center->w < d);
}
