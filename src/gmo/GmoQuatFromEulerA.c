// bdc 0x08a13d10 GmoQuatFromEulerA
#include "bdc.h"

/* Converts Euler angles (radians, x/y/z at `euler`) to a quaternion (x, y, z, w) stored at `out`:
   half-angle sines/cosines (`vsin`/`vcos` of `angle / pi` in quarter turns) combined as
   w = cx*cy*cz + sx*sy*sz and x/y/z with the sign pattern [-, +, -] of rotation order A.
   Returns `out`. */
float *GmoQuatFromEulerA(float *out, const float *euler)
{
    float ax = euler[0] * 0.318309873f;
    float ay = euler[1] * 0.318309873f;
    float az = euler[2] * 0.318309873f;
    float cx = VfCosQuarter(ax);
    float cy = VfCosQuarter(ay);
    float cz = VfCosQuarter(az);
    float sx = VfSinQuarter(ax);
    float sy = VfSinQuarter(ay);
    float sz = VfSinQuarter(az);
    float cyz = cy * cz;
    float czx = cz * cx;
    float cxy = cx * cy;
    float syz = sy * sz;
    float szx = sz * sx;
    float sxy = sx * sy;
    float w = cyz * cx + syz * sx;

    out[0] = cyz * sx - syz * cx;
    out[1] = czx * sy + szx * cy;
    out[2] = cxy * sz - sxy * cz;
    out[3] = w;
    return out;
}
