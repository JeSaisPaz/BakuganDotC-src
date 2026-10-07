// bdc 0x088e160c ActorPlayerUpdateMatrix
#include "bdc.h"

/* Column-major 4x4 product `out = a * b` (the VFPU `vmmul.q`): element (row i, column j) is
   b[j][0] * a[0][i] + b[j][1] * a[1][i] + b[j][2] * a[2][i] + b[j][3] * a[3][i], summed left to
   right. Both inputs are read in full before `out` is written, so `out` may alias either. */
static void Mat4Mul(float *out, const float *a, const float *b)
{
    float r[16];
    int i;
    int j;

    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            r[j * 4 + i] = b[j * 4 + 0] * a[0 * 4 + i] + b[j * 4 + 1] * a[1 * 4 + i] +
                           b[j * 4 + 2] * a[2 * 4 + i] + b[j * 4 + 3] * a[3 * 4 + i];
        }
    }
    for (i = 0; i < 16; i++) {
        out[i] = r[i];
    }
}

/* Builds the model world matrix of the player actor class (model 0x2f `12_Edit_man.gmo`, 0x550
   bytes, constructor `ActorPlayerCtor`, vtable `0x08af38e4`) (vtable slot 10, `+0x54`; the base
   class uses `ActorUpdateMatrix`): orientation `tilt` from the ground-tilt vector `tiltQuat`
   (`MathQuatFromUpToDir` -> rotation matrix), times a Y rotation by `pi/2 - heading` (`rot[1]`,
   wrapped into (-pi, pi]) times diag(`scale`), translated to `pos + modelOffset` (w forced to 1),
   written into the model data's `rootMatrix`. While a ball is held (`ball` and `handNode` set),
   the ball's `rootMatrix` becomes `rootMatrix * handNode->localMatrix * hold`, where `hold` swaps
   Y/Z (y' = -z, z' = y) and offsets by (6, 2.5, -2). All matrices column-major. */

void ActorPlayerUpdateMatrix(ActorPlayer *self)

{
    float quat[4];
    float qa[16];
    float qb[16];
    float tilt[16];
    float rotY[16];
    float scl[16];
    float hand[16];
    float held[16];
    float hold[16];
    const float *q;
    float *dst;
    float *ballMtx;
    float angle;
    float c;
    float s;
    float x;
    float y;
    float z;
    float w;
    int i;

    /* Quaternion -> rotation matrix: tilt = qa * qb with sign-swizzled copies of q as columns,
       then row 3 and column 3 set to the identity's. */
    q = MathQuatFromUpToDir(quat, self->base.tiltQuat);
    x = q[0];
    y = q[1];
    z = q[2];
    w = q[3];
    qa[0] = w;   qa[1] = z;   qa[2] = -y;  qa[3] = -x;
    qa[4] = -z;  qa[5] = w;   qa[6] = x;   qa[7] = -y;
    qa[8] = y;   qa[9] = -x;  qa[10] = w;  qa[11] = -z;
    qa[12] = x;  qa[13] = y;  qa[14] = z;  qa[15] = w;
    qb[0] = w;   qb[1] = z;   qb[2] = -y;  qb[3] = x;
    qb[4] = -z;  qb[5] = w;   qb[6] = x;   qb[7] = y;
    qb[8] = y;   qb[9] = -x;  qb[10] = w;  qb[11] = z;
    qb[12] = -x; qb[13] = -y; qb[14] = -z; qb[15] = w;
    Mat4Mul(tilt, qa, qb);
    tilt[3] = 0.0f;
    tilt[7] = 0.0f;
    tilt[11] = 0.0f;
    tilt[12] = 0.0f;
    tilt[13] = 0.0f;
    tilt[14] = 0.0f;
    tilt[15] = 1.0f;

    /* rootMatrix = rotY(pi/2 - heading) * diag(scale.x, scale.y, scale.z, 1). */
    dst = self->base.base.data->rootMatrix;
    angle = 1.5707964f - self->base.base.rot[1];
    if (!(angle <= 3.1415927f)) {
        angle = angle - 6.2831855f;
    } else if (angle <= -3.1415927f) {
        angle = angle + 6.2831855f;
    }
    for (i = 0; i < 16; i++) {
        scl[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }
    scl[0] = self->base.base.scale[0];
    scl[5] = self->base.base.scale[1];
    scl[10] = self->base.base.scale[2];
    /* vrot of angle * 2/pi (S703) in quarter turns: plain cos/sin of the angle. */
    c = __builtin_cosf(angle);
    s = __builtin_sinf(angle);
    rotY[0] = c;     rotY[1] = 0.0f;  rotY[2] = -s;    rotY[3] = 0.0f;
    rotY[4] = 0.0f;  rotY[5] = 1.0f;  rotY[6] = 0.0f;  rotY[7] = 0.0f;
    rotY[8] = s;     rotY[9] = 0.0f;  rotY[10] = c;    rotY[11] = 0.0f;
    rotY[12] = 0.0f; rotY[13] = 0.0f; rotY[14] = 0.0f; rotY[15] = 1.0f;
    Mat4Mul(dst, rotY, scl);

    /* rootMatrix = tilt * rootMatrix. */
    dst = self->base.base.data->rootMatrix;
    Mat4Mul(dst, tilt, dst);

    /* Translation = pos (all four lanes), then xyz += modelOffset; w forced to 1. */
    dst = self->base.base.data->rootMatrix;
    dst[12] = self->base.base.pos[0];
    dst[13] = self->base.base.pos[1];
    dst[14] = self->base.base.pos[2];
    dst[15] = self->base.base.pos[3];
    dst = self->base.base.data->rootMatrix;
    dst[12] = dst[12] + self->modelOffset[0];
    dst[13] = dst[13] + self->modelOffset[1];
    dst[14] = dst[14] + self->modelOffset[2];
    self->base.base.data->rootMatrix[15] = 1.0f;

    if (self->ball != NULL && self->handNode != NULL) {
        /* Column-major hold matrix: x' = x + 6, y' = -z + 2.5, z' = y - 2. */
        hold[0] = 1.0f;
        hold[1] = 0.0f;
        hold[2] = 0.0f;
        hold[3] = 0.0f;
        hold[4] = 0.0f;
        hold[5] = 0.0f;
        hold[6] = 1.0f;
        hold[7] = 0.0f;
        hold[8] = 0.0f;
        hold[9] = -1.0f;
        hold[10] = 0.0f;
        hold[11] = 0.0f;
        hold[12] = 6.0f;
        hold[13] = 2.5f;
        hold[14] = -2.0f;
        hold[15] = 1.0f;
        ballMtx = ((GfxModel *)self->ball)->data->rootMatrix;
        Mat4Mul(hand, self->base.base.data->rootMatrix, ((GmoNode *)self->handNode)->localMatrix);
        Mat4Mul(held, hand, hold);
        for (i = 0; i < 16; i++) {
            ballMtx[i] = held[i];
        }
    }
}
