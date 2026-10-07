// bdc 0x089e45a0 CollisionTriangleTestSphere
#include "bdc.h"

/* Sphere/triangle test for query type 3: closest point of the triangle `abc` to the sphere centre
   (query vec4 at +0x10, w = squared radius at +0x1c) via `CollisionClosestPointOnTriangle`
   (feature code into g_collisionMeshQuery.closestOnEdge, point into g_collisionMeshQuery.point),
   stores the squared distance in `*distSq` and returns whether it is within the squared radius
   (true when !(radiusSq < distSq)). */

bool CollisionTriangleTestSphere(const float *a, const float *b, const float *c, const void *query, float *distSq)
{
    const float *center = (const float *)query + 4;
    float dx, dy, dz, d;

    g_collisionMeshQuery.closestOnEdge = CollisionClosestPointOnTriangle(a, b, c, center, &g_collisionMeshQuery.point.x);
    dx = g_collisionMeshQuery.point.x - center[0];
    dy = g_collisionMeshQuery.point.y - center[1];
    dz = g_collisionMeshQuery.point.z - center[2];
    d = dx * dx + dy * dy + dz * dz;
    *distSq = d;
    return !(center[3] < d);
}
