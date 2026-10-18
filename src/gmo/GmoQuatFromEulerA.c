// bdc 0x08a13d10 GmoQuatFromEulerA
#include "bdc.h"

/* Converts Euler angles (radians, x/y/z at `euler`) to a quaternion (x, y, z, w) stored at `out`:
   half-angle sines/cosines (`vsin`/`vcos` of `angle / pi` in quarter turns) combined as
   w = cx*cy*cz + sx*sy*sz and x/y/z with the sign pattern [-, +, -] of rotation order A.
   Returns `out`. */
float *GmoQuatFromEulerA(float *out, const float *euler)
{
    float ax = VfMul(euler[0], 0.318309873f);
    float ay = VfMul(euler[1], 0.318309873f);
    float az = VfMul(euler[2], 0.318309873f);
    float cx = VfCosQuarter(ax);
    float cy = VfCosQuarter(ay);
    float cz = VfCosQuarter(az);
    float sx = VfSinQuarter(ax);
    float sy = VfSinQuarter(ay);
    float sz = VfSinQuarter(az);
    float cyz = VfMul(cy, cz);
    float czx = VfMul(cz, cx);
    float cxy = VfMul(cx, cy);
    float syz = VfMul(sy, sz);
    float szx = VfMul(sz, sx);
    float sxy = VfMul(sx, sy);
    float w = VfAdd(VfMul(cyz, cx), VfMul(syz, sx));

    out[0] = VfSub(VfMul(cyz, sx), VfMul(syz, cx));
    out[1] = VfAdd(VfMul(czx, sy), VfMul(szx, cy));
    out[2] = VfSub(VfMul(cxy, sz), VfMul(sxy, cz));
    out[3] = w;
    return out;
}
