// bdc 0x089e4188 CollisionClosestPointOnTriangle
#include "bdc.h"

/* Closest point on triangle `abc` to point `p` (the Voronoi-region method from Ericson's *Real-Time
   Collision Detection*, VFPU `vsub.t`/`vdot.t`): writes the point (vec4) to `out` and returns the
   feature it lies on: 2 for a vertex region, 1 for an edge region, 0 for the face interior. Used by
   the triangle tests `CollisionTriangleTestSphere` and `CollisionTriangleTestSweptSphere`. */

/* out.xyz = x.xyz - y.xyz, out.w = x.w (vsub.t keeps the loaded w lane). */
static inline void Vec3Sub(float *out, const float *x, const float *y)
{
    out[0] = x[0] - y[0];
    out[1] = x[1] - y[1];
    out[2] = x[2] - y[2];
    out[3] = x[3];
}

/* x.xyz . y.xyz (vdot.t). */
static inline float Vec3Dot(const float *x, const float *y)
{
    return x[0] * y[0] + x[1] * y[1] + x[2] * y[2];
}

/* out.xyz = base.xyz + dir.xyz * s, out.w = base.w (vscl.t then vadd.t). */
static inline void Vec3AddScaled(float *out, const float *base, const float *dir, float s)
{
    float scaled[3];

    scaled[0] = dir[0] * s;
    scaled[1] = dir[1] * s;
    scaled[2] = dir[2] * s;
    out[0] = base[0] + scaled[0];
    out[1] = base[1] + scaled[1];
    out[2] = base[2] + scaled[2];
    out[3] = base[3];
}

static inline void Vec4Copy(float *out, const float *src)
{
    out[0] = src[0];
    out[1] = src[1];
    out[2] = src[2];
    out[3] = src[3];
}

s32 CollisionClosestPointOnTriangle(const float *a, const float *b, const float *c, const float *p, float *out)
{
    float ab[4];
    float ac[4];
    float ap[4];
    float bp[4];
    float cp[4];
    float bc[4];
    float tmp[4];
    float d1, d2, d3, d4, d5, d6;
    float va, vb, vc, denom, bcStart, bcEnd;

    Vec3Sub(ab, b, a);
    Vec3Sub(ac, c, a);
    Vec3Sub(ap, p, a);
    d1 = Vec3Dot(ab, ap);
    d2 = Vec3Dot(ac, ap);
    if (d1 <= 0.0f && d2 <= 0.0f) {
        Vec4Copy(out, a);
        return 2;
    }

    Vec3Sub(bp, p, b);
    d3 = Vec3Dot(ab, bp);
    d4 = Vec3Dot(ac, bp);
    if (!(d3 < 0.0f) && !(d3 < d4)) {
        Vec4Copy(out, b);
        return 2;
    }

    vc = d1 * d4 - d2 * d3;
    if (vc <= 0.0f && !(d1 < 0.0f) && d3 <= 0.0f) {
        Vec3AddScaled(out, a, ab, d1 / (d1 - d3));
        return 1;
    }

    Vec3Sub(cp, p, c);
    d5 = Vec3Dot(ab, cp);
    d6 = Vec3Dot(ac, cp);
    if (!(d6 < 0.0f) && d5 <= d6) {
        Vec4Copy(out, c);
        return 2;
    }

    vb = d2 * d5 - d1 * d6;
    if (vb <= 0.0f && !(d2 < 0.0f) && d6 <= 0.0f) {
        Vec3AddScaled(out, a, ac, d2 / (d2 - d6));
        return 1;
    }

    va = d3 * d6 - d4 * d5;
    if (va <= 0.0f) {
        bcStart = d4 - d3;
        if (!(bcStart < 0.0f)) {
            bcEnd = d5 - d6;
            if (!(bcEnd < 0.0f)) {
                Vec3Sub(bc, c, b);
                Vec3AddScaled(out, b, bc, bcStart / (bcStart + bcEnd));
                return 1;
            }
        }
    }

    denom = 1.0f / (va + vb + vc);
    Vec3AddScaled(tmp, a, ab, denom * vb);
    Vec3AddScaled(out, tmp, ac, denom * vc);
    return 0;
}
