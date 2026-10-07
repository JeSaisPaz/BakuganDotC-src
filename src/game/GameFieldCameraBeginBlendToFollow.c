// bdc 0x088bcc08 GameFieldCameraBeginBlendToFollow
#include "bdc.h"

/* Starts a smooth return of the field camera (`GameFieldCameraCtor`) to the follow view: saves
   the current eye/look-at as blend start (`+0x370`/`+0x380`), snaps a reset and runs
   `GameFieldCameraModeFollow` 10 times to get the end view (`+0x390`/`+0x3a0`), restores the
   current view, sets the blend factor `+0x360 = 1` and switches to mode 6
   (`GameFieldCameraModeBlend`). */

static void CopyVec4(float *dst, const float *src)
{
  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
}

void GameFieldCameraBeginBlendToFollow(GameFieldCamera *cam)

{
  s32 i;

  CopyVec4(&cam->blendEyeFrom.x, cam->base.eye);
  CopyVec4(&cam->blendLookFrom.x, cam->base.target);
  cam->blend = 1.0f;
  GameFieldCameraReset(cam, 1, 0);
  for (i = 0; i < 10; i++) {
    GameFieldCameraModeFollow(cam);
  }
  CopyVec4(&cam->blendEyeTo.x, cam->base.eye);
  CopyVec4(&cam->blendLookTo.x, cam->base.target);
  CopyVec4(cam->base.eye, &cam->blendEyeFrom.x);
  CopyVec4(cam->base.target, &cam->blendLookFrom.x);
  GameFieldCameraSetMode(cam, 6);
}
