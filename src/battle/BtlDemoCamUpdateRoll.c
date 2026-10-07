// bdc 0x088fde9c BtlDemoCamUpdateRoll
#include "bdc.h"

/* Rolls the battle demo camera by `roll`: takes the XZ heading from `eye` to `target`
   (`atan2f``(dz, dx)`), builds the rolled up vector `(sin roll, cos roll, 0, 0)` and turns it
   about Y by heading + pi/2, eases `up` 30% (0.300000012) of the way toward it (all four lanes)
   and renormalises its xyz, clamping each lane to [-1, 1] (a zero-length vector scales by 0);
   `up[3]` ends up 0 (the masked w lane of C710, bank constant S713). */
void BtlDemoCamUpdateRoll(float roll, BtlDemoCam *self)
{
    float *up = self->base.up;
    float dx = self->base.target[0] - self->base.eye[0];
    float dz = self->base.target[2] - self->base.eye[2];
    float turn = atan2f(dz, dx) + 1.57079637f;
    float rs = __builtin_sinf(roll);
    float rc = __builtin_cosf(roll);
    float ca = __builtin_cosf(turn);
    float sa = __builtin_sinf(turn);
    float rolled[4];
    float len;
    int i;

    /* rotate (rs, rc, 0) about Y: x = dot(v, (cos, 0, -sin)), z = dot(v, (sin, 0, cos)) */
    rolled[0] = rs * ca + rc * 0.0f + 0.0f * -sa;
    rolled[1] = rc;
    rolled[2] = rs * sa + rc * 0.0f + 0.0f * ca;
    rolled[3] = 0.0f;

    for (i = 0; i < 4; i++) {
        up[i] = up[i] + (rolled[i] - up[i]) * 0.300000012f;
    }

    len = up[0] * up[0] + up[1] * up[1] + up[2] * up[2];
    if (len == 0.0f) {
        len = 0.0f;
    } else {
        len = VfRsq(len);
    }
    up[0] = VfSat1(up[0] * len);
    up[1] = VfSat1(up[1] * len);
    up[2] = VfSat1(up[2] * len);
    up[3] = 0.0f;
}
