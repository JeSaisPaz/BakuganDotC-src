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
        float ax = v.x * 0.318309873f;
        float ay = v.y * 0.318309873f;
        float az = v.z * 0.318309873f;
        float cx = VfCosQuarter(ax);
        float sx = VfSinQuarter(ax);
        float cy = VfCosQuarter(ay);
        float sy = VfSinQuarter(ay);
        float cz = VfCosQuarter(az);
        float sz = VfSinQuarter(az);
        float cxy = cx * cy;
        float sxy = sx * sy;
        ScePspFVector4 a;
        ScePspFVector4 b;

        a.x = (cy * cz) * sx;
        a.y = (cz * cx) * sy;
        a.z = cxy * sz;
        a.w = cxy * cz;
        b.x = (sy * sz) * cx;
        b.y = (sz * sx) * cy;
        b.z = sxy * cz;
        b.w = sxy * sz;
        if (type == 0x49) {
            v.x = a.x + -b.x;
            v.y = a.y + b.y;
        } else {
            v.x = a.x + b.x;
            v.y = a.y + -b.y;
        }
        v.z = a.z + -b.z;
        v.w = a.w + b.w;
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

            v.x = d0 + (v.x - d0) * weight;
            v.y = d1 + (v.y - d1) * weight;
            v.z = d2 + (v.z - d2) * weight;
            v.w = d3 + (v.w - d3) * weight;
        }
    }
    dst[0] = v.x;
    dst[1] = v.y;
    dst[2] = v.z;
    dst[3] = v.w;
}
