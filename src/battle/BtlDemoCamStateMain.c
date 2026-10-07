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
  float y;
  float z;
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
  lensT = lensT < 1.0f ? lensT : 1.0f;
  lensT = lensT > 0.0f ? lensT : 0.0f;
  eyeT = eyeT < 1.0f ? eyeT : 1.0f;
  eyeT = eyeT > 0.0f ? eyeT : 0.0f;
  keyBT = keyBT < 1.0f ? keyBT : 1.0f;
  keyBT = keyBT > 0.0f ? keyBT : 0.0f;

  if (lensT == 1.0f) {
    for (i = 0; i < 4; i++) {
      eye[i] = self->lensKey[0][i];
    }
    fovScale = self->lensA[0];
    roll = self->lensB[0];
  }
  else {
    for (i = 0; i < 4; i++) {
      eye[i] = self->lensKey[1][i] + (self->lensKey[0][i] - self->lensKey[1][i]) * lensT;
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
      target[i] = self->eyeKey[1][i] + (self->eyeKey[0][i] - self->eyeKey[1][i]) * eyeT;
    }
    angleX = self->lookKey[1][0] + (self->lookKey[0][0] - self->lookKey[1][0]) * eyeT;
  }

  if (keyBT == 1.0f) {
    for (i = 0; i < 3; i++) {
      offset[i] = self->keyBA[0][i];
    }
  }
  else {
    for (i = 0; i < 3; i++) {
      offset[i] = self->keyBA[1][i] + (self->keyBA[0][i] - self->keyBA[1][i]) * keyBT;
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
  c = __builtin_cosf(angleX);
  s = __builtin_sinf(angleX);
  y = target[1];
  z = target[2];
  target[1] = y * c - z * s;
  target[2] = y * s + z * c;
  y = eye[1];
  z = eye[2];
  eye[1] = y * c - z * s;
  eye[2] = y * s + z * c;

  if (self->fixedOffset != 0) {
    for (i = 0; i < 3; i++) {
      self->base.eye[i] = eye[i] + offset[i];
    }
    self->base.eye[3] = eye[3];
    for (i = 0; i < 3; i++) {
      self->base.target[i] = target[i] + offset[i];
    }
    self->base.target[3] = target[3];
    fov = self->fov;
  }
  else {
    BtlGetStageAmbientColor(amb);
    self->base.eye[0] = (eye[0] + amb[0]) + g_btlDemoCamVector.x;
    self->base.eye[1] = (eye[1] + amb[1]) + g_btlDemoCamVector.y;
    self->base.eye[2] = (eye[2] + amb[2]) + g_btlDemoCamVector.z;
    self->base.eye[3] = eye[3];
    BtlGetStageAmbientColor(amb);
    self->base.target[0] = (target[0] + amb[0]) + g_btlDemoCamVector.x;
    self->base.target[1] = (target[1] + amb[1]) + g_btlDemoCamVector.y;
    self->base.target[2] = (target[2] + amb[2]) + g_btlDemoCamVector.z;
    self->base.target[3] = target[3];
    eyeY = self->base.eye[1];
    self->base.target[1] = eyeY + self->lookHeightBlend * (self->base.target[1] - eyeY);
    fov = self->fov;
  }
  rollScale = self->rollScale;
  self->base.fov = fov * fovScale;
  BtlDemoCamUpdateRoll(roll * rollScale, self);
}
