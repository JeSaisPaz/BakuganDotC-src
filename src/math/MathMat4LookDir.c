// bdc 0x08a2971c MathMat4LookDir
#include "bdc.h"

/* Builds an orientation matrix from a direction and an up vector: z = normalise(`dir`),
   x = normalise(`up` x z), y = z x x, w = (0, 0, 0, 1); each axis is stored as a row of `out`
   (w components 0). The two normalised axes are clamped to [-1, 1] per component, and a
   zero-length vector is scaled by 0 instead of infinity. Returns `out`. */
float *MathMat4LookDir(float *out, const float *dir, const float *up)
{
    float s;
    float zx, zy, zz;
    float xx, xy, xz;
    float yx, yy, yz;

    s = dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2];
    if (s == 0.0f)
        s = 0.0f;
    else
        s = VfRsq(s);
    zx = VfSat1(dir[0] * s);
    zy = VfSat1(dir[1] * s);
    zz = VfSat1(dir[2] * s);

    xx = up[1] * zz - up[2] * zy;
    xy = up[2] * zx - up[0] * zz;
    xz = up[0] * zy - up[1] * zx;
    s = xx * xx + xy * xy + xz * xz;
    if (s == 0.0f)
        s = 0.0f;
    else
        s = VfRsq(s);
    xx = VfSat1(xx * s);
    xy = VfSat1(xy * s);
    xz = VfSat1(xz * s);

    yx = zy * xz - zz * xy;
    yy = zz * xx - zx * xz;
    yz = zx * xy - zy * xx;

    out[0] = xx;
    out[1] = xy;
    out[2] = xz;
    out[3] = 0.0f;
    out[4] = yx;
    out[5] = yy;
    out[6] = yz;
    out[7] = 0.0f;
    out[8] = zx;
    out[9] = zy;
    out[10] = zz;
    out[11] = 0.0f;
    out[12] = 0.0f;
    out[13] = 0.0f;
    out[14] = 0.0f;
    out[15] = 1.0f;
    return out;
}
