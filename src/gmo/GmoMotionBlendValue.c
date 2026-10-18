// bdc 0x089dde80 GmoMotionBlendValue
#include "bdc.h"

/* Writes an evaluated channel value `v` (the vector the caller's key evaluation left in VFPU C000)
   into `dst`, all four lanes, blended by `weight` when `blend` is set. Channel types 0x49/0x4a
   (Euler rotations, two axis orders) are first converted to a quaternion (half angles: each lane
   times 1/π in quarter turns) and treated as type 0x4b; quaternions blend with `GmoQuatSlerpInto`,
   other channels linearly (`d + (v - d) * weight`). */

void GmoMotionBlendValue(float weight, float *dst, bool blend, s32 type, ScePspFVector4 v)
{
    if (type == 0x49 || type == 0x4a) {
        float ax = VfMul(v.x, 0.318309873f);
        float ay = VfMul(v.y, 0.318309873f);
        float az = VfMul(v.z, 0.318309873f);
        float cx = VfCosQuarter(ax);
        float sx = VfSinQuarter(ax);
        float cy = VfCosQuarter(ay);
        float sy = VfSinQuarter(ay);
        float cz = VfCosQuarter(az);
        float sz = VfSinQuarter(az);
        float cxy = VfMul(cx, cy);
        float sxy = VfMul(sx, sy);
        ScePspFVector4 a;
        ScePspFVector4 b;

        a.x = VfMul(VfMul(cy, cz), sx);
        a.y = VfMul(VfMul(cz, cx), sy);
        a.z = VfMul(cxy, sz);
        a.w = VfMul(cxy, cz);
        b.x = VfMul(VfMul(sy, sz), cx);
        b.y = VfMul(VfMul(sz, sx), cy);
        b.z = VfMul(sxy, cz);
        b.w = VfMul(sxy, sz);
        if (type == 0x49) {
            v.x = VfAdd(a.x, -b.x);
            v.y = VfAdd(a.y, b.y);
        } else {
            v.x = VfAdd(a.x, b.x);
            v.y = VfAdd(a.y, -b.y);
        }
        v.z = VfAdd(a.z, -b.z);
        v.w = VfAdd(a.w, b.w);
        type = 0x4b;
    }
    if (blend) {
        if (type == 0x4b) {
            v = GmoQuatSlerpInto(weight, dst, v);
        } else {
            float d0 = dst[0];
            float d1 = dst[1];
            float d2 = dst[2];
            float d3 = dst[3];

            v.x = VfAdd(d0, VfMul(VfSub(v.x, d0), weight));
            v.y = VfAdd(d1, VfMul(VfSub(v.y, d1), weight));
            v.z = VfAdd(d2, VfMul(VfSub(v.z, d2), weight));
            v.w = VfAdd(d3, VfMul(VfSub(v.w, d3), weight));
        }
    }
    dst[0] = v.x;
    dst[1] = v.y;
    dst[2] = v.z;
    dst[3] = v.w;
}
