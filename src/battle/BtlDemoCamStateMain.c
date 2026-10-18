// bdc 0x088fe1c4 BtlDemoCamStateMain
#include "bdc.h"

/* State 0 handler of the battle demo camera (`BtlDemoCamCtor`, dispatched by `BtlDemoCamUpdate`
   through the member-pointer table `0x08a99888`): computes three progress values of the current
   frame `frame` within the key windows `eyeFrame`, `lensFrame` and `keyBFrame` (1 for an empty
   window, 0 while the window's end frame is 0), clamps them to [0, 1], interpolates the key pairs
   pushed by `BtlDemoCamPushEyeKey`/`BtlDemoCamPushKeyB`/`BtlDemoCamPushLensKey`
   (`lensKey` -> camera eye, `lensA`/`lensB` -> FOV scale and roll, `eyeKey` -> camera target,
   `lookKey` -> negated rotation angle, `keyBA` -> offset; `keyBB` is interpolated by the binary but
   never used), applies per-demo tweaks by `shotId` (offsets, FOV scale 1.15..1.4, zeroed X angle,
   `nearZ` 3), rotates eye and target about the X axis, then either adds the `keyBA` offset
   (`fixedOffset` set) or the stage vector (`BtlGetStageAmbientColor`) plus `g_btlDemoCamVector`
   (blending the target height towards the eye by `lookHeightBlend`), stores `base.fov` = `fov` x
   scale and sets the roll (`BtlDemoCamUpdateRoll`). */

void BtlDemoCamStateMain(BtlDemoCam *self)
{
  float eye[4];
  float target[4];
  float offset[3];
  float amb[4];
  float angleX;
  float eyeT;
  float keyBT;
  float lensT;
  float fovScale;
  float roll;
  float fov;
  float rollScale;
  float eyeY;
  float c;
  float s;
  float v[4];
  float m1[16];
  float m2[16];
  float m3[16];
  s32 eyeSpan;
  s32 lensSpan;
  s32 keyBSpan;
  s32 i;

  lensT = 0.0f;
  eyeT = 0.0f;
  keyBT = 0.0f;
  eyeSpan = self->eyeFrame[0] - self->eyeFrame[1];
  lensSpan = self->lensFrame[0] - self->lensFrame[1];
  keyBSpan = self->keyBFrame[1] - self->keyBFrame[0];
  if (eyeSpan == 0) {
    eyeT = 1.0f;
  }
  else if (self->eyeFrame[0] != 0) {
    eyeT = (float)(eyeSpan - (self->eyeFrame[0] - self->frame) + 1) / (float)eyeSpan;
  }
  if (keyBSpan == 0) {
    keyBT = 1.0f;
  }
  else if (self->keyBFrame[1] != 0) {
    keyBT = (float)(keyBSpan - (self->keyBFrame[1] - self->frame) + 1) / (float)keyBSpan;
  }
  if (lensSpan == 0) {
    lensT = 1.0f;
  }
  else if (self->lensFrame[0] != 0) {
    lensT = (float)(lensSpan - (self->lensFrame[0] - self->frame) + 1) / (float)lensSpan;
  }

  /* vmin.s against S733 (1.0f), then vmax.s against S713 (0.0f). The quotients of two ints with a
     non-zero divisor are never NaN; a tie returns the second operand, so -0.0f becomes +0.0f. */
  lensT = VfMin(lensT, 1.0f);
  lensT = VfMax(lensT, 0.0f);
  eyeT = VfMin(eyeT, 1.0f);
  eyeT = VfMax(eyeT, 0.0f);
  keyBT = VfMin(keyBT, 1.0f);
  keyBT = VfMax(keyBT, 0.0f);

  if (lensT == 1.0f) {
    for (i = 0; i < 4; i++) {
      eye[i] = self->lensKey[0][i];
    }
    fovScale = self->lensA[0];
    roll = self->lensB[0];
  }
  else {
    for (i = 0; i < 4; i++) {
      eye[i] = VfAdd(self->lensKey[1][i], VfMul(VfSub(self->lensKey[0][i], self->lensKey[1][i]), lensT));
    }
    fovScale = self->lensA[0] * lensT + self->lensA[1] * (1.0f - lensT);
    roll = self->lensB[0] * lensT + self->lensB[1] * (1.0f - lensT);
  }

  /* Only the X lane of the look angles is used: the Y/Z rotations below read the identity. */
  if (eyeT == 1.0f) {
    for (i = 0; i < 4; i++) {
      target[i] = self->eyeKey[0][i];
    }
    angleX = self->lookKey[0][0];
  }
  else {
    for (i = 0; i < 4; i++) {
      target[i] = VfAdd(self->eyeKey[1][i], VfMul(VfSub(self->eyeKey[0][i], self->eyeKey[1][i]), eyeT));
    }
    angleX = VfAdd(self->lookKey[1][0], VfMul(VfSub(self->lookKey[0][0], self->lookKey[1][0]), eyeT));
  }

  if (keyBT == 1.0f) {
    for (i = 0; i < 3; i++) {
      offset[i] = self->keyBA[0][i];
    }
  }
  else {
    for (i = 0; i < 3; i++) {
      offset[i] = VfAdd(self->keyBA[1][i], VfMul(VfSub(self->keyBA[0][i], self->keyBA[1][i]), keyBT));
    }
  }

  angleX = -angleX;

  if (self->shotId < 0x3e6) {
    switch (self->shotId) {
    case 3:
    case 4:
      eye[2] = eye[2] + 10.0f;
      break;
    case 6:
    case 18:
      target[1] = target[1] + 1.0f;
      fovScale = fovScale * 1.2f;
      eye[2] = eye[2] + 10.0f;
      break;
    case 14:
      eye[2] = eye[2] + 2.0f;
      break;
    case 15:
      eye[2] = eye[2] + 5.0f;
      break;
    case 25: case 33: case 37: case 41: case 45: case 49: case 53: case 57: case 65:
    case 69: case 73: case 77: case 81: case 85: case 89: case 93: case 97:
    case 88: case 92: case 94:
      angleX = 0.0f;
      break;
    case 27:
    case 55:
    case 70: case 74: case 78:
      fovScale = fovScale * 1.2f;
      break;
    case 29:
    case 61:
      eye[1] = 10.0f;
      break;
    case 35:
      eye[0] = eye[0] - 1.0f;
      fovScale = fovScale * 1.4f;
      break;
    case 38:
      angleX = 0.0f;
      fovScale = fovScale * 1.4f;
      break;
    case 43:
    case 91:
      self->base.nearZ = 3.0f;
      angleX = 0.0f;
      fovScale = fovScale * 1.2f;
      break;
    case 51:
      eye[0] = eye[0] - 2.0f;
      target[0] = target[0] + 12.0f;
      break;
    case 63:
      eye[0] = eye[0] + 1.0f;
      break;
    case 71:
    case 75:
      angleX = 0.0f;
      fovScale = fovScale * 1.2f;
      break;
    case 101:
      fovScale = fovScale * 1.15f;
      angleX = 0.0f;
      target[1] = target[1] + 0.26f;
      break;
    case 103:
      break;
    case 104:
      target[1] = 40.0f;
      break;
    default:
      break;
    }
  }

  /* The binary builds identity x Rx(angleX) x Ry x Rz with vmidt/vrot/vmmul, but the Y and Z
     vrot read S101/S102 of the identity just loaded into M100 (0), so the product is Rx(angleX)
     (angle scaled by S703 = 2/pi into quarter turns). vtfm4 then maps x and w unchanged. */
  c = VfCosQuarter(VfMul(angleX, 0.636619747f));
  s = VfSinQuarter(VfMul(angleX, 0.636619747f));
  /* m1 = I x Rx(angleX); m2 = m1 x Ry(S101 = 0); m3 = m2 x Rz(S102 = 0): vrot of 0 gives cos 1, sin 0, -sin -0. */
  m1[1] = VfDot2(0.0f, 0.0f, c, -s);
  m1[2] = VfDot2(0.0f, 0.0f, s, c);
  m1[5] = VfDot2(1.0f, 0.0f, c, -s);
  m1[6] = VfDot2(1.0f, 0.0f, s, c);
  m1[9] = VfDot2(0.0f, 1.0f, c, -s);
  m1[10] = VfDot2(0.0f, 1.0f, s, c);
  m1[13] = VfDot2(0.0f, 0.0f, c, -s);
  m1[14] = VfDot2(0.0f, 0.0f, s, c);
  m2[0] = VfDot3(1.0f, m1[1], m1[2], 1.0f, 0.0f, 0.0f);
  m2[1] = VfDot2(m1[1], m1[2], 1.0f, 0.0f);
  m2[2] = VfDot2(m1[1], m1[2], 0.0f, 1.0f);
  m2[3] = VfDot2(m1[1], m1[2], 0.0f, 0.0f);
  m2[4] = VfDot2(m1[5], m1[6], 0.0f, 0.0f);
  m2[5] = VfDot2(m1[5], m1[6], 1.0f, 0.0f);
  m2[6] = VfDot2(m1[5], m1[6], 0.0f, 1.0f);
  m2[7] = VfDot2(m1[5], m1[6], 0.0f, 0.0f);
  m2[8] = VfDot2(m1[9], m1[10], 0.0f, 0.0f);
  m2[9] = VfDot2(m1[9], m1[10], 1.0f, 0.0f);
  m2[10] = VfDot2(m1[9], m1[10], 0.0f, 1.0f);
  m2[11] = VfDot2(m1[9], m1[10], 0.0f, 0.0f);
  m2[12] = VfDot2(m1[13], m1[14], 0.0f, 0.0f);
  m2[13] = VfDot2(m1[13], m1[14], 1.0f, 0.0f);
  m2[14] = VfDot2(m1[13], m1[14], 0.0f, 1.0f);
  m2[15] = VfDot3(m1[13], m1[14], 1.0f, 0.0f, 0.0f, 1.0f);
  m3[0] = VfDot4(m2[0], m2[1], m2[2], m2[3], 1.0f, -0.0f, 0.0f, 0.0f);
  m3[1] = VfDot4(m2[0], m2[1], m2[2], m2[3], 0.0f, 1.0f, 0.0f, 0.0f);
  m3[2] = VfDot4(m2[0], m2[1], m2[2], m2[3], 0.0f, 0.0f, 1.0f, 0.0f);
  m3[3] = VfDot4(m2[0], m2[1], m2[2], m2[3], 0.0f, 0.0f, 0.0f, 1.0f);
  m3[4] = VfDot4(m2[4], m2[5], m2[6], m2[7], 1.0f, -0.0f, 0.0f, 0.0f);
  m3[5] = VfDot4(m2[4], m2[5], m2[6], m2[7], 0.0f, 1.0f, 0.0f, 0.0f);
  m3[6] = VfDot4(m2[4], m2[5], m2[6], m2[7], 0.0f, 0.0f, 1.0f, 0.0f);
  m3[7] = VfDot4(m2[4], m2[5], m2[6], m2[7], 0.0f, 0.0f, 0.0f, 1.0f);
  m3[8] = VfDot4(m2[8], m2[9], m2[10], m2[11], 1.0f, -0.0f, 0.0f, 0.0f);
  m3[9] = VfDot4(m2[8], m2[9], m2[10], m2[11], 0.0f, 1.0f, 0.0f, 0.0f);
  m3[10] = VfDot4(m2[8], m2[9], m2[10], m2[11], 0.0f, 0.0f, 1.0f, 0.0f);
  m3[11] = VfDot4(m2[8], m2[9], m2[10], m2[11], 0.0f, 0.0f, 0.0f, 1.0f);
  m3[12] = VfDot4(m2[12], m2[13], m2[14], m2[15], 1.0f, -0.0f, 0.0f, 0.0f);
  m3[13] = VfDot4(m2[12], m2[13], m2[14], m2[15], 0.0f, 1.0f, 0.0f, 0.0f);
  m3[14] = VfDot4(m2[12], m2[13], m2[14], m2[15], 0.0f, 0.0f, 1.0f, 0.0f);
  m3[15] = VfDot4(m2[12], m2[13], m2[14], m2[15], 0.0f, 0.0f, 0.0f, 1.0f);
  for (i = 0; i < 4; i++) {
    v[i] = target[i];
  }
  target[0] = VfDot4(v[0], v[1], v[2], v[3], m3[0], m3[4], m3[8], m3[12]);
  target[1] = VfDot4(v[0], v[1], v[2], v[3], m3[1], m3[5], m3[9], m3[13]);
  target[2] = VfDot4(v[0], v[1], v[2], v[3], m3[2], m3[6], m3[10], m3[14]);
  target[3] = VfDot4(v[0], v[1], v[2], v[3], m3[3], m3[7], m3[11], m3[15]);
  for (i = 0; i < 4; i++) {
    v[i] = eye[i];
  }
  eye[0] = VfDot4(v[0], v[1], v[2], v[3], m3[0], m3[4], m3[8], m3[12]);
  eye[1] = VfDot4(v[0], v[1], v[2], v[3], m3[1], m3[5], m3[9], m3[13]);
  eye[2] = VfDot4(v[0], v[1], v[2], v[3], m3[2], m3[6], m3[10], m3[14]);
  eye[3] = VfDot4(v[0], v[1], v[2], v[3], m3[3], m3[7], m3[11], m3[15]);

  if (self->fixedOffset != 0) {
    for (i = 0; i < 3; i++) {
      self->base.eye[i] = VfAdd(eye[i], offset[i]);
    }
    self->base.eye[3] = eye[3];
    for (i = 0; i < 3; i++) {
      self->base.target[i] = VfAdd(target[i], offset[i]);
    }
    self->base.target[3] = target[3];
    fov = self->fov;
  }
  else {
    BtlGetStageAmbientColor(amb);
    self->base.eye[0] = VfAdd(VfAdd(eye[0], amb[0]), g_btlDemoCamVector.x);
    self->base.eye[1] = VfAdd(VfAdd(eye[1], amb[1]), g_btlDemoCamVector.y);
    self->base.eye[2] = VfAdd(VfAdd(eye[2], amb[2]), g_btlDemoCamVector.z);
    self->base.eye[3] = eye[3];
    BtlGetStageAmbientColor(amb);
    self->base.target[0] = VfAdd(VfAdd(target[0], amb[0]), g_btlDemoCamVector.x);
    self->base.target[1] = VfAdd(VfAdd(target[1], amb[1]), g_btlDemoCamVector.y);
    self->base.target[2] = VfAdd(VfAdd(target[2], amb[2]), g_btlDemoCamVector.z);
    self->base.target[3] = target[3];
    eyeY = self->base.eye[1];
    self->base.target[1] = eyeY + self->lookHeightBlend * (self->base.target[1] - eyeY);
    fov = self->fov;
  }
  rollScale = self->rollScale;
  self->base.fov = fov * fovScale;
  BtlDemoCamUpdateRoll(roll * rollScale, self);
}
