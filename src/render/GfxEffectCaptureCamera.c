// bdc 0x088237f8 GfxEffectCaptureCamera
#include "bdc.h"

/* Snapshots the current camera (`g_gfxActiveCamera`) for the effect update: copies its `yaw`
   to `g_gfxEffectCamYaw` and its `dir` and `eye` vectors (all four lanes) to `g_gfxEffectCamDir` /
   `g_gfxEffectCamEye`, and resets the per-tick update counter `g_gfxEffectCaptureCounter`. Called at
   the start of every effect-manager update (`GfxEffectMgrUpdate`, `GfxEffectMgrUpdateOwner`) and by
   `BtlFinishTaskUpdate`. */

void GfxEffectCaptureCamera(void)
{
  GfxCamera *cam = g_gfxActiveCamera;

  g_gfxEffectCamYaw = cam->yaw;
  g_gfxEffectCamDir[0] = cam->dir[0];
  g_gfxEffectCamDir[1] = cam->dir[1];
  g_gfxEffectCamDir[2] = cam->dir[2];
  g_gfxEffectCamDir[3] = cam->dir[3];
  cam = g_gfxActiveCamera;
  g_gfxEffectCamEye[0] = cam->eye[0];
  g_gfxEffectCamEye[1] = cam->eye[1];
  g_gfxEffectCamEye[2] = cam->eye[2];
  g_gfxEffectCamEye[3] = cam->eye[3];
  g_gfxEffectCaptureCounter = 0;
}
