// bdc 0x088bd438 GameFieldCameraModeBlend
#include "bdc.h"

/* Mode 6 update of the field camera (`GameFieldCameraCtor`)
   (`GameFieldCameraBeginBlendToFollow`): when the camera has a target, decreases the blend factor
   `blend` by 1/16 per frame (clamped to [0, 1]), eases with `k = (1 - cos(π t²)) / 2` between the
   saved start (`blendEyeTo`/`blendLookTo`) and end views (`blendEyeFrom`/`blendLookFrom`) for eye and
   look-at (`to + (from - to) * k`), and switches to mode 0 when `blend` reaches 0 (`blend <= 0`).
   Without a target it does nothing. */

void GameFieldCameraModeBlend(GameFieldCamera *cam)

{
  float t;
  float k;
  float d[4];

  if (cam->target != NULL) {
    t = cam->blend - 0.0625f;
    /* vmin.s against S733 (1.0f), then vmax.s against S713 (0.0f); a tie gives the second operand,
       +NaN loses vmin (1), -NaN wins vmin then loses vmax (0). */
    if (t != t) {
      t = __builtin_signbit(t) ? 0.0f : 1.0f;
    } else {
      t = t <= 0.0f ? 0.0f : t > 1.0f ? 1.0f : t;
    }
    cam->blend = t;
    /* vcos.s of (t * t * π) * S703 (2/π): the cosine of t * t * π radians. */
    k = (1.0f - __builtin_cosf(t * t * 3.1415927f)) * 0.5f;

    d[0] = (cam->blendEyeFrom.x - cam->blendEyeTo.x) * k;
    d[1] = (cam->blendEyeFrom.y - cam->blendEyeTo.y) * k;
    d[2] = (cam->blendEyeFrom.z - cam->blendEyeTo.z) * k;
    d[3] = (cam->blendEyeFrom.w - cam->blendEyeTo.w) * k;
    cam->base.eye[0] = cam->blendEyeTo.x + d[0];
    cam->base.eye[1] = cam->blendEyeTo.y + d[1];
    cam->base.eye[2] = cam->blendEyeTo.z + d[2];
    cam->base.eye[3] = cam->blendEyeTo.w + d[3];

    d[0] = (cam->blendLookFrom.x - cam->blendLookTo.x) * k;
    d[1] = (cam->blendLookFrom.y - cam->blendLookTo.y) * k;
    d[2] = (cam->blendLookFrom.z - cam->blendLookTo.z) * k;
    d[3] = (cam->blendLookFrom.w - cam->blendLookTo.w) * k;
    cam->base.target[0] = cam->blendLookTo.x + d[0];
    cam->base.target[1] = cam->blendLookTo.y + d[1];
    cam->base.target[2] = cam->blendLookTo.z + d[2];
    cam->base.target[3] = cam->blendLookTo.w + d[3];

    if (cam->blend <= 0.0f) {
      GameFieldCameraSetMode(cam, 0);
    }
  }
}
