// bdc 0x088c926c GameFieldCameraHeadViewUpdate
#include "bdc.h"

/* Per-frame step of the field camera's head view helper (`cam+0x3d0`, a
   `GameFieldCameraSpringCtor` holder plus target vectors at `+0x10`/`+0x20`): springs eye/look-at
   towards the targets (`GameFieldCameraSpringStep`, damping 0.8); while the eye is within 50
   units of its target (squared distance < 2500) it also runs a wall test
   (`GameFieldCameraProbeCollide`) that writes `outEye`/`outLook`; otherwise `outEye` is the sprung
   eye. `outLook` is then always overwritten with the sprung look-at. */

typedef struct {
  u8 spring[0x10];
  float targetLook[4];
  float targetEye[4];
} GameFieldHeadView;

void GameFieldCameraHeadViewUpdate(void *view, float *outEye, float *outLook)

{
  GameFieldHeadView *hv = (GameFieldHeadView *)view;
  float eye[4];
  float look[4];
  u8 probe[16];
  float dx, dy, dz;
  float dist2;

  GameFieldCameraSpringStep(0.8f, view, eye, look, hv->targetEye, hv->targetLook);
  dx = eye[0] - hv->targetEye[0];
  dy = eye[1] - hv->targetEye[1];
  dz = eye[2] - hv->targetEye[2];
  dist2 = dx * dx;
  dist2 = dist2 + dy * dy;
  dist2 = dist2 + dz * dz;
  if (dist2 < 2500.0f) {
    GameFieldCameraProbeCtor(probe);
    GameFieldCameraProbeCollide(probe, outEye, outLook, eye, hv->targetLook);
    GameFieldCameraProbeDtor(probe, 2);
  } else {
    outEye[0] = eye[0];
    outEye[1] = eye[1];
    outEye[2] = eye[2];
    outEye[3] = eye[3];
  }
  outLook[0] = look[0];
  outLook[1] = look[1];
  outLook[2] = look[2];
  outLook[3] = look[3];
}
