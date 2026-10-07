// bdc 0x089e29b4 GfxCameraInit
#include "bdc.h"

/* Resets a camera to defaults: field of view 40, eye (0, 0, -10), target 0, up (0, 1, 0),
   screen offset, fog base, yaw, depth range 0..65535, cleared shake state, frustum scale 1 and
   frustum offset 0, near 6.5 / far 4000, both validity flags cleared, GMO eye vector
   (0.1846, 0.1046, 1, 1), identity projection and view matrices, then `GfxCameraUpdate` with
   all flags. Returns nothing. The view direction `dir` is zeroed (VFPU bank constant C720 = 0). */

void GfxCameraInit(GfxCamera *cam)
{
  cam->fov = 40.0f;
  cam->eye[0] = 0.0f;
  cam->eye[1] = 0.0f;
  cam->eye[2] = -10.0f;
  cam->eye[3] = 0.0f;
  cam->target[0] = 0.0f;
  cam->target[1] = 0.0f;
  cam->target[2] = 0.0f;
  cam->target[3] = 0.0f;
  cam->up[0] = 0.0f;
  cam->up[1] = 1.0f;
  cam->up[2] = 0.0f;
  cam->up[3] = 0.0f;
  cam->screenOffset[0] = 0.0f;
  cam->screenOffset[1] = 0.0f;
  cam->fogBase = 0.0f;
  cam->depthMax = 65535.0f;
  cam->depthMin = 0.0f;
  cam->yaw = 0.0f;
  cam->dir[0] = 0.0f;
  cam->dir[1] = 0.0f;
  cam->dir[2] = 0.0f;
  cam->dir[3] = 0.0f;
  cam->shakeFrames = 0;
  cam->shakeAmp = 0.0f;
  cam->shakeSpeed = 0.0f;
  cam->shakePhase = 0.0f;
  cam->shakeDecay = 0.0f;
  cam->frustumScale = 1.0f;
  cam->frustumOffset[0] = 0.0f;
  cam->frustumOffset[1] = 0.0f;
  cam->nearZ = 6.5f;
  cam->farZ = 4000.0f;
  cam->viewProjValid = 0;
  cam->maybe_matrixValid = 0;
  cam->gmoEye[0] = 0.18461539f;
  cam->gmoEye[1] = 0.10461538f;
  cam->gmoEye[2] = 1.0f;
  cam->gmoEye[3] = 1.0f;
  cam->proj.x.x = 1.0f; cam->proj.x.y = 0.0f; cam->proj.x.z = 0.0f; cam->proj.x.w = 0.0f;
  cam->proj.y.x = 0.0f; cam->proj.y.y = 1.0f; cam->proj.y.z = 0.0f; cam->proj.y.w = 0.0f;
  cam->proj.z.x = 0.0f; cam->proj.z.y = 0.0f; cam->proj.z.z = 1.0f; cam->proj.z.w = 0.0f;
  cam->proj.w.x = 0.0f; cam->proj.w.y = 0.0f; cam->proj.w.z = 0.0f; cam->proj.w.w = 1.0f;
  cam->view.x.x = 1.0f; cam->view.x.y = 0.0f; cam->view.x.z = 0.0f; cam->view.x.w = 0.0f;
  cam->view.y.x = 0.0f; cam->view.y.y = 1.0f; cam->view.y.z = 0.0f; cam->view.y.w = 0.0f;
  cam->view.z.x = 0.0f; cam->view.z.y = 0.0f; cam->view.z.z = 1.0f; cam->view.z.w = 0.0f;
  cam->view.w.x = 0.0f; cam->view.w.y = 0.0f; cam->view.w.z = 0.0f; cam->view.w.w = 1.0f;
  GfxCameraUpdate(cam, 0xffffffffu);
}
