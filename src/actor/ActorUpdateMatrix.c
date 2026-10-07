// bdc 0x088de8ec ActorUpdateMatrix
#include "bdc.h"

/* Matrix method of the actors (vtable slot 10, shared by the base and 5 NPC classes; the player
   class uses `ActorPlayerUpdateMatrix`): builds the model data's `rootMatrix` as
   tilt * diag(scale) * rotY(pi/2 - heading), where tilt is the rotation matrix of the quaternion
   `MathQuatFromUpToDir` builds from `tiltQuat`, the heading is `rot[1]` (angle wrapped into
   (-pi, pi]), then sets the translation row to `pos` and forces its w to 1. */

void ActorUpdateMatrix(Actor *self)
{
    float quat[4];
    float tilt[16];
    float a[4][4];
    float b[3][4];
    float m[16];
    const float *q;
    float *dst;
    float angle;
    float c;
    float s;
    int i;
    int j;

    /* Quaternion -> rotation matrix: column j of tilt = sum_k b[j][k] * a[k], with a and b the
       sign-swizzled copies of q (a[3] = q); row 3 and column 3 are then set to the identity's. */
    q = MathQuatFromUpToDir(quat, self->tiltQuat);
    a[0][0] = q[3];  a[0][1] = q[2];  a[0][2] = -q[1]; a[0][3] = -q[0];
    a[1][0] = -q[2]; a[1][1] = q[3];  a[1][2] = q[0];  a[1][3] = -q[1];
    a[2][0] = q[1];  a[2][1] = -q[0]; a[2][2] = q[3];  a[2][3] = -q[2];
    a[3][0] = q[0];  a[3][1] = q[1];  a[3][2] = q[2];  a[3][3] = q[3];
    b[0][0] = q[3];  b[0][1] = q[2];  b[0][2] = -q[1]; b[0][3] = q[0];
    b[1][0] = -q[2]; b[1][1] = q[3];  b[1][2] = q[0];  b[1][3] = q[1];
    b[2][0] = q[1];  b[2][1] = -q[0]; b[2][2] = q[3];  b[2][3] = q[2];
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 3; i++) {
            tilt[j * 4 + i] = b[j][0] * a[0][i] + b[j][1] * a[1][i] + b[j][2] * a[2][i] +
                              b[j][3] * a[3][i];
        }
        tilt[j * 4 + 3] = 0.0f;
    }
    tilt[12] = 0.0f;
    tilt[13] = 0.0f;
    tilt[14] = 0.0f;
    tilt[15] = 1.0f;

    /* rootMatrix = diag(scale) * rotY(pi/2 - heading). */
    angle = 1.5707964f - self->base.rot[1];
    if (!(angle <= 3.14159274f)) {
        angle = angle - 6.28318548f;
    } else if (angle <= -3.14159274f) {
        angle = angle + 6.28318548f;
    }
    dst = self->base.data->rootMatrix;
    c = __builtin_cosf(angle);
    s = __builtin_sinf(angle);
    dst[0] = self->base.scale[0] * c;
    dst[1] = 0.0f;
    dst[2] = self->base.scale[0] * -s;
    dst[3] = 0.0f;
    dst[4] = 0.0f;
    dst[5] = self->base.scale[1];
    dst[6] = 0.0f;
    dst[7] = 0.0f;
    dst[8] = self->base.scale[2] * s;
    dst[9] = 0.0f;
    dst[10] = self->base.scale[2] * c;
    dst[11] = 0.0f;
    dst[12] = 0.0f;
    dst[13] = 0.0f;
    dst[14] = 0.0f;
    dst[15] = 1.0f;

    /* rootMatrix = tilt * rootMatrix (column j = sum_k rootMatrix[j][k] * tilt column k). */
    dst = self->base.data->rootMatrix;
    for (i = 0; i < 16; i++) {
        m[i] = dst[i];
    }
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            dst[j * 4 + i] = m[j * 4 + 0] * tilt[0 + i] + m[j * 4 + 1] * tilt[4 + i] +
                             m[j * 4 + 2] * tilt[8 + i] + m[j * 4 + 3] * tilt[12 + i];
        }
    }

    /* Translation row = pos (quad copy), w forced to 1. */
    dst = self->base.data->rootMatrix;
    dst[12] = self->base.pos[0];
    dst[13] = self->base.pos[1];
    dst[14] = self->base.pos[2];
    dst[15] = self->base.pos[3];
    self->base.data->rootMatrix[15] = 1.0f;
}
