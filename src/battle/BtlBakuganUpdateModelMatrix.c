// bdc 0x0885fa90 BtlBakuganUpdateModelMatrix
#include "bdc.h"

/* Rebuilds the root matrix of the unit's model (`base.data->rootMatrix`, four columns of four
   floats): a tilt matrix from the quaternion that rotates +Y onto the direction `orient`
   (`MathQuatFromUpToDir`), combined with the scale (`base.scale`) and yaw `pi/2 - base.rot[1]`
   (wrapped into (-pi, pi]) matrix; the translation column is then set to `base.pos` with w = 1. */
void BtlBakuganUpdateModelMatrix(BtlBakugan *self)
{
    float quat[4];
    float tilt[16];
    float lhs[4][4];
    float rhs[4][4];
    float col[4];
    float *q;
    float *m;
    float yaw;
    float c;
    float s;
    int i;
    int j;

    q = MathQuatFromUpToDir(quat, self->orient);
    /* Quaternion -> rotation matrix as the product of its left and right multiplication
       matrices; the w row and column are reset to identity. */
    lhs[0][0] = q[3];  lhs[0][1] = q[2];  lhs[0][2] = -q[1]; lhs[0][3] = -q[0];
    lhs[1][0] = -q[2]; lhs[1][1] = q[3];  lhs[1][2] = q[0];  lhs[1][3] = -q[1];
    lhs[2][0] = q[1];  lhs[2][1] = -q[0]; lhs[2][2] = q[3];  lhs[2][3] = -q[2];
    lhs[3][0] = q[0];  lhs[3][1] = q[1];  lhs[3][2] = q[2];  lhs[3][3] = q[3];
    rhs[0][0] = q[3];  rhs[0][1] = q[2];  rhs[0][2] = -q[1]; rhs[0][3] = q[0];
    rhs[1][0] = -q[2]; rhs[1][1] = q[3];  rhs[1][2] = q[0];  rhs[1][3] = q[1];
    rhs[2][0] = q[1];  rhs[2][1] = -q[0]; rhs[2][2] = q[3];  rhs[2][3] = q[2];
    rhs[3][0] = -q[0]; rhs[3][1] = -q[1]; rhs[3][2] = -q[2]; rhs[3][3] = q[3];
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 3; i++) {
            tilt[j * 4 + i] = VfDot4(lhs[0][i], lhs[1][i], lhs[2][i], lhs[3][i], rhs[j][0], rhs[j][1], rhs[j][2], rhs[j][3]);
        }
        tilt[j * 4 + 3] = 0.0f;
    }
    tilt[12] = 0.0f;
    tilt[13] = 0.0f;
    tilt[14] = 0.0f;
    tilt[15] = 1.0f;

    yaw = 1.57079637f - self->base.rot[1];
    if (yaw <= 3.14159274f) {
        if (yaw <= -3.14159274f) {
            yaw = yaw + 6.28318548f;
        }
    } else {
        yaw = yaw - 6.28318548f;
    }
    /* rootMatrix = rotY(yaw) with column k scaled by scale[k] (vrot of yaw * 2/pi). */
    c = VfCosQuarter(VfMul(yaw, 0.636619747f));
    s = VfSinQuarter(VfMul(yaw, 0.636619747f));
    m = self->base.data->rootMatrix;
    m[0] = VfDot2(self->base.scale[0], 0.0f, c, s);
    m[1] = VfDot2(self->base.scale[0], 0.0f, 0.0f, 0.0f);
    m[2] = VfDot2(self->base.scale[0], 0.0f, -s, c);
    m[3] = VfDot2(self->base.scale[0], 0.0f, 0.0f, 0.0f);
    m[4] = VfDot3(0.0f, self->base.scale[1], 0.0f, c, 0.0f, s);
    m[5] = VfDot2(self->base.scale[1], 0.0f, 1.0f, 0.0f);
    m[6] = VfDot3(0.0f, self->base.scale[1], 0.0f, -s, 0.0f, c);
    m[7] = VfDot2(self->base.scale[1], 0.0f, 0.0f, 0.0f);
    m[8] = VfDot2(0.0f, self->base.scale[2], c, s);
    m[9] = VfDot2(self->base.scale[2], 0.0f, 0.0f, 0.0f);
    m[10] = VfDot2(0.0f, self->base.scale[2], -s, c);
    m[11] = VfDot2(self->base.scale[2], 0.0f, 0.0f, 0.0f);
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 1.0f;

    /* rootMatrix = tilt * rootMatrix (column-major). Column 3 is overwritten below, so only
       columns 0-2 are computed. */
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 4; i++) {
            col[i] = m[j * 4 + i];
        }
        for (i = 0; i < 4; i++) {
            m[j * 4 + i] = VfDot4(tilt[0 * 4 + i], tilt[1 * 4 + i], tilt[2 * 4 + i], tilt[3 * 4 + i], col[0], col[1], col[2], col[3]);
        }
    }
    /* Translation column = base.pos, w = 1. */
    m[12] = self->base.pos[0];
    m[13] = self->base.pos[1];
    m[14] = self->base.pos[2];
    m[15] = 1.0f;
}
