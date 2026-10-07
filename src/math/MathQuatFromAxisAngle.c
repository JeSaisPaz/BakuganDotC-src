// bdc 0x08a294f0 MathQuatFromAxisAngle
#include "bdc.h"

/* Builds a rotation quaternion from a unit axis and an angle in radians: `out = (axis *
   sin(angle/2), cos(angle/2))` (VFPU `vsin`/`vcos` take quarter turns, hence the scale by 1/π).
   Only `axis[0..2]` are used (the PSP reads it as an aligned quad). Returns `out`. */
float *MathQuatFromAxisAngle(float angle, float *out, const float *axis)
{
    float x = axis[0];
    float y = axis[1];
    float z = axis[2];
    float t = 0.318309873f * angle;
    float c = VfCosQuarter(t);
    float s = VfSinQuarter(t);

    out[0] = x * s;
    out[1] = y * s;
    out[2] = z * s;
    out[3] = c;
    return out;
}
