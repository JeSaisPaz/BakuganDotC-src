// bdc 0x08847b28 BtlCameraUpdateFollow
#include "bdc.h"

/* Per-frame update of the battle follow camera (mode 2, set up by `BtlCameraSetFollowTarget`).
   Does nothing without `followTarget`. Advances `eyeSnap` by 0.05 (capped at 1.0) and eases it as
   `t = (1 - cos(eyeSnap^2 * pi)) / 2`. The focus starts at (0, 0, 0, 0); with a `followPos`,
   `followFocus` first moves 8% of the way toward `*followPos` and becomes the focus.
   `followHeight` is added to the focus Y. The yaw is `followYawSrc[1]`, or (no source)
   `atan2f(eye.z - focus.z, eye.x - focus.x)`; `followOffset3f4` plus a wobble
   (`followWobble -= 0.008`, `0.4 * sin(followWobble)`) is added and the yaw is wrapped once into
   (-pi, pi]. The new eye is `focus + (cos yaw, 0, sin yaw) * (followDistance +
   followDistanceOffset)` with `followOffsetY` added to Y (w = 0). `target` blends from
   `savedTarget` to the focus and `eye` from `savedEye` to the new eye by `t`; `fov` blends from
   `savedFov` to `followFov` by `t` and is clamped to [5, 60]. */

void BtlCameraUpdateFollow(BtlCamera *camera)
{
  float focus[4];
  float eyePos[4];
  float blend;
  float t;
  float yaw;
  float wobble;
  float dist;
  float fov;
  int i;

  if (camera->followTarget == NULL) {
    return;
  }
  blend = camera->eyeSnap + 0.0500000007f;
  if (!(blend <= 1.0f)) {
    blend = 1.0f;
  }
  camera->eyeSnap = blend;
  t = (1.0f - __builtin_cosf(blend * blend * 3.14159274f)) * 0.5f;

  /* focus starts as the bank zero vector C720 */
  focus[0] = 0.0f;
  focus[1] = 0.0f;
  focus[2] = 0.0f;
  focus[3] = 0.0f;
  if (camera->followPos != NULL) {
    for (i = 0; i < 4; i++) {
      camera->followFocus[i] =
          camera->followFocus[i] + (camera->followPos[i] - camera->followFocus[i]) * 0.08f;
    }
    for (i = 0; i < 4; i++) {
      focus[i] = camera->followFocus[i];
    }
  }
  focus[1] = focus[1] + camera->followHeight;

  if (camera->followYawSrc != NULL) {
    yaw = camera->followYawSrc[1];
  } else {
    yaw = atan2f(camera->base.eye[2] - focus[2], camera->base.eye[0] - focus[0]);
  }
  yaw = yaw + camera->followOffset3f4;
  wobble = camera->followWobble - 0.00800000038f;
  camera->followWobble = wobble;
  yaw = yaw + __builtin_sinf(wobble) * 0.400000006f;
  if (!(yaw <= 3.14159274f)) {
    yaw = yaw - 6.28318548f;
  } else if (yaw <= -3.14159274f) {
    yaw = yaw + 6.28318548f;
  }

  /* vrot [C,0,S,0] scaled (vscl.t) by dist, then xyz + focus; w stays 0 */
  dist = camera->followDistance + camera->followDistanceOffset;
  eyePos[0] = __builtin_cosf(yaw) * dist + focus[0];
  eyePos[1] = 0.0f * dist + focus[1];
  eyePos[2] = __builtin_sinf(yaw) * dist + focus[2];
  eyePos[3] = 0.0f;
  eyePos[1] = eyePos[1] + camera->followOffsetY;

  for (i = 0; i < 4; i++) {
    camera->base.target[i] =
        camera->savedTarget[i] + (focus[i] - camera->savedTarget[i]) * t;
  }
  for (i = 0; i < 4; i++) {
    camera->base.eye[i] = camera->savedEye[i] + (eyePos[i] - camera->savedEye[i]) * t;
  }

  fov = camera->savedFov + (camera->followFov - camera->savedFov) * t;
  camera->base.fov = fov;
  if (fov < 5.0f) {
    fov = 5.0f;
  } else if (!(fov <= 60.0f)) {
    fov = 60.0f;
  }
  camera->base.fov = fov;
}
