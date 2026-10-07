// bdc 0x08a2951c MathQuatFromUpToDir
#include "bdc.h"

/* Builds the quaternion that rotates the +Y axis onto the unit vector `dir`: axis =
   normalize(`dir.z`, 0, `-dir.x`) (= Y × dir), angle = `acosf(dir.y)`. When `dir` is
   (anti)parallel to Y (`x² + z² < 1e-5` and `!(y² <= 1e-4)`) it stores the identity quaternion
   (0, 0, 0, 1) (the bank's C730). Returns `out`. */
float *MathQuatFromUpToDir(float *out, const float *dir)
{
    float axis[3];
    float lenSq;
    float invLen;
    float angle;
    float halfTurns;
    float s;
    bool alongY;

    if (dir[0] * dir[0] + dir[2] * dir[2] < 1e-05f) {
        alongY = !(dir[1] * dir[1] <= 0.0001f);
    } else {
        alongY = false;
    }

    if (alongY) {
        /* sv.q of the bank's C730 = (0, 0, 0, 1). */
        out[0] = 0.0f;
        out[1] = 0.0f;
        out[2] = 0.0f;
        out[3] = 1.0f;
        return out;
    }

    axis[0] = dir[2];
    axis[1] = 0.0f;
    axis[2] = -dir[0];
    /* Normalise the axis (rsqrt; a zero length uses S713 = 0 instead), clamped to [-1, 1]. */
    lenSq = axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2];
    if (lenSq == 0.0f) {
        invLen = 0.0f;
    } else {
        invLen = VfRsq(lenSq);
    }
    axis[0] = VfSat1(axis[0] * invLen);
    axis[1] = VfSat1(axis[1] * invLen);
    axis[2] = VfSat1(axis[2] * invLen);

    angle = acosf(dir[1]);

    /* angle / π quarter turns = angle / 2 radians: out = (axis * sin(angle / 2), cos(angle / 2)). */
    halfTurns = 0.318309873f * angle;
    out[3] = VfCosQuarter(halfTurns);
    s = VfSinQuarter(halfTurns);
    out[0] = axis[0] * s;
    out[1] = axis[1] * s;
    out[2] = axis[2] * s;
    return out;
}
