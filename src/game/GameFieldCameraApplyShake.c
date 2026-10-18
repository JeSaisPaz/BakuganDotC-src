// bdc 0x088b9adc GameFieldCameraApplyShake
#include "bdc.h"

/* Builds the final view of the field camera (`GameFieldCameraCtor`): while the shake counter
   `shakeTimer` runs, adds π to `shakeAngle` every frame and sets `shakeRight`/`shakeUp` to
   `shakeAmplitude * cos/sin(angle)`, counting the timer down; at 0 or below it resets the timer
   to 0, angle π/2, offsets 0 and amplitude 1. Then adds the previous frame's `shakeOffset` to eye
   and look-at, stores view, proj*view and its rigid inverse in `viewMatrix[0..2]`, and computes
   the new `shakeOffset = right * shakeRight + up * shakeUp` (w = 0) from the inverse's first two
   columns. */

/* Column `col` of `a * b` (VFPU `vmmul.q M000, M100, M200` with M100 = a, M200 = b). */
static void GameFieldCameraMulColumn(ScePspFVector4 *d, const ScePspFMatrix4 *a,
                                     const ScePspFVector4 *col)
{
  d->x = VfDot4(col->x, col->y, col->z, col->w, a->x.x, a->y.x, a->z.x, a->w.x);
  d->y = VfDot4(col->x, col->y, col->z, col->w, a->x.y, a->y.y, a->z.y, a->w.y);
  d->z = VfDot4(col->x, col->y, col->z, col->w, a->x.z, a->y.z, a->z.z, a->w.z);
  d->w = VfDot4(col->x, col->y, col->z, col->w, a->x.w, a->y.w, a->z.w, a->w.w);
}

void GameFieldCameraApplyShake(GameFieldCamera *cam)
{
  ScePspFMatrix4 viewProj;
  ScePspFMatrix4 *inv;
  float right[3];
  float up[3];
  float tx, ty, tz;
  float angle;
  float amp;
  int i;

  if (cam->shakeTimer <= 0) {
    cam->shakeTimer = 0;
    cam->shakeAngle = 1.57079637f;
    cam->shakeRight = 0.0f;
    cam->shakeUp = 0.0f;
    cam->shakeAmplitude = 1.0f;
  } else {
    angle = cam->shakeAngle + 3.14159274f;
    cam->shakeAngle = angle;
    /* vcos/vsin of angle * S703 (2/π): cos/sin of the angle in radians */
    amp = cam->shakeAmplitude;
    cam->shakeRight = amp * VfCosQuarter(VfMul(angle, 0.636619747f));
    cam->shakeUp = amp * VfSinQuarter(VfMul(angle, 0.636619747f));
    cam->shakeTimer = cam->shakeTimer - 1;
  }
  /* eye += shakeOffset, look-at += shakeOffset (xyz; w kept) */
  cam->base.eye[0] = VfAdd(cam->base.eye[0], cam->shakeOffset.x);
  cam->base.eye[1] = VfAdd(cam->base.eye[1], cam->shakeOffset.y);
  cam->base.eye[2] = VfAdd(cam->base.eye[2], cam->shakeOffset.z);
  cam->base.target[0] = VfAdd(cam->base.target[0], cam->shakeOffset.x);
  cam->base.target[1] = VfAdd(cam->base.target[1], cam->shakeOffset.y);
  cam->base.target[2] = VfAdd(cam->base.target[2], cam->shakeOffset.z);
  /* viewMatrix[0] = view; viewMatrix[1] = viewMatrix[2] = proj * view */
  cam->viewMatrix[0] = cam->base.view;
  GameFieldCameraMulColumn(&viewProj.x, &cam->base.proj, &cam->base.view.x);
  GameFieldCameraMulColumn(&viewProj.y, &cam->base.proj, &cam->base.view.y);
  GameFieldCameraMulColumn(&viewProj.z, &cam->base.proj, &cam->base.view.z);
  GameFieldCameraMulColumn(&viewProj.w, &cam->base.proj, &cam->base.view.w);
  cam->viewMatrix[1] = viewProj;
  cam->viewMatrix[2] = cam->viewMatrix[1];
  /* viewMatrix[2] = rigid inverse: 3x3 part transposed (w words 0), translation = -(R^T * t),
     translation w kept */
  inv = &cam->viewMatrix[2];
  viewProj = *inv;
  tx = VfDot3(viewProj.x.x, viewProj.x.y, viewProj.x.z, viewProj.w.x, viewProj.w.y, viewProj.w.z);
  ty = VfDot3(viewProj.y.x, viewProj.y.y, viewProj.y.z, viewProj.w.x, viewProj.w.y, viewProj.w.z);
  tz = VfDot3(viewProj.z.x, viewProj.z.y, viewProj.z.z, viewProj.w.x, viewProj.w.y, viewProj.w.z);
  inv->x.x = viewProj.x.x;
  inv->x.y = viewProj.y.x;
  inv->x.z = viewProj.z.x;
  inv->x.w = 0.0f;
  inv->y.x = viewProj.x.y;
  inv->y.y = viewProj.y.y;
  inv->y.z = viewProj.z.y;
  inv->y.w = 0.0f;
  inv->z.x = viewProj.x.z;
  inv->z.y = viewProj.y.z;
  inv->z.z = viewProj.z.z;
  inv->z.w = 0.0f;
  inv->w.x = -tx;
  inv->w.y = -ty;
  inv->w.z = -tz;
  /* right = first column, up = second column of the inverse (xyz) */
  right[0] = inv->x.x;
  up[0] = inv->x.y;
  right[1] = inv->y.x;
  up[1] = inv->y.y;
  right[2] = inv->z.x;
  up[2] = inv->z.y;
  /* shakeOffset = right * shakeRight + up * shakeUp; w = 0 (the bank's S713 through vscl.t C710) */
  for (i = 0; i < 3; i++) {
    right[i] = VfMul(right[i], cam->shakeRight);
  }
  for (i = 0; i < 3; i++) {
    up[i] = VfMul(up[i], cam->shakeUp);
  }
  cam->shakeOffset.x = VfAdd(right[0], up[0]);
  cam->shakeOffset.y = VfAdd(right[1], up[1]);
  cam->shakeOffset.z = VfAdd(right[2], up[2]);
  cam->shakeOffset.w = 0.0f;
}
