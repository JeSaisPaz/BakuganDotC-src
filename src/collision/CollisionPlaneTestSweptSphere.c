// bdc 0x089e48b0 CollisionPlaneTestSweptSphere
#include "bdc.h"

/* Plane test for a swept sphere (`CollisionCapsule` query: `start`, `axis`, `radius`; plane =
   `normal.xyz` with `d` at `plane[3]`). If the start centre is within `radius` of the plane,
   stores t = 0 and copies `start` (lane 3 = `radiusSq`) to `point`, returns 1. Otherwise, when the
   motion `axis` approaches the plane (dot(normal, axis) * dist < 0), stores the time of contact
   `*t = (±radius - dist) / dot(normal, axis)` (denominator clamped away from 0 by 1e-5 keeping its
   sign) and, if `*t <= 1`, writes the contact point `start + axis * t - normal * ±radius` (lane 3 =
   `radiusSq`) and returns 1. Returns 0 when moving away/parallel or `*t > 1` (`*t` is still
   written in the latter case). */

bool CollisionPlaneTestSweptSphere(const float *plane, const void *query, float *t, float *point)
{
    const CollisionCapsule *q = query;
    union {
        float f;
        u32 u;
    } speed;
    float dist;
    float r;
    float denom;
    float tt;

    /* dist = dot(normal, start) - d */
    dist = plane[0] * q->start[0] + plane[1] * q->start[1] + plane[2] * q->start[2];
    dist = dist - plane[3];
    if (fabsf(dist) <= q->radius) {
        *t = 0.0f;
        /* 16-byte copy: lane 3 is the word after start (radiusSq) */
        point[0] = q->start[0];
        point[1] = q->start[1];
        point[2] = q->start[2];
        point[3] = q->radiusSq;
        return true;
    }

    /* speed = dot(normal, axis) */
    speed.f = plane[0] * q->axis[0] + plane[1] * q->axis[1] + plane[2] * q->axis[2];
    if (!(speed.f * dist < 0.0f)) {
        return false;
    }
    if (dist <= 0.0f) {
        r = -q->radius;
    } else {
        r = q->radius;
    }
    denom = speed.f;
    if (speed.u <= 0x80000000u) {
        /* +x or -0.0 */
        if (speed.f < 1e-05f) {
            denom = 1e-05f;
        }
    } else if (!(speed.f <= -1e-05f)) {
        denom = -1e-05f;
    }
    *t = (r - dist) / denom;
    if (!(*t <= 1.0f)) {
        return false;
    }

    tt = *t;
    /* point = (start + axis * t) - normal * r; lane 3 keeps start's (radiusSq). */
    point[0] = (q->start[0] + q->axis[0] * tt) - plane[0] * r;
    point[1] = (q->start[1] + q->axis[1] * tt) - plane[1] * r;
    point[2] = (q->start[2] + q->axis[2] * tt) - plane[2] * r;
    point[3] = q->radiusSq;
    return true;
}
