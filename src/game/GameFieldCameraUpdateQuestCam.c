// bdc 0x088bb2e0 GameFieldCameraUpdateQuestCam
#include "bdc.h"

/* Steps the quest camera controller `+0x5c4` (`GameQuestCamCtrlCtor`) of the field camera
   (embedded at `+0x20` of the field scene task; follow target `+0x2a0`, eye `+0x50`, look-at
   `+0x60`) and adopts its view: sets the controller target in the parameter object `+0x5c8`
   (`+0x10`) to a point 10 units ahead of the follow target along its heading and 12 units up, ×10
   (the quest tables use 10× world units), runs the controller's vtable slot `+0x14` with dt 1/30,
   then copies its eye (`+0x00`) and look-at (`+0x10`) ×0.1 into the camera eye `+0x50` / look-at
   `+0x60` (and `+0x2c0`), recomputes the yaw `+0x2f0` (`atan2f`) and takes near/far from controller
   `+0x20`/`+0x24`. No-op without a controller or target.
   The w lanes written (heading point, controller target, eye, look-at) come from the bank zero
   S713 and are 0. */

void GameFieldCameraUpdateQuestCam(GameFieldCamera *cam)
{
  float ahead[4];
  float *params;
  GameQuestCamCtrl *ctrl;
  Actor *target;
  const VtblEntry *step;
  float heading;

  if (cam->questCam == NULL || cam->target == NULL) {
    return;
  }
  /* ahead = (cos, 0, sin)(heading) * 10 + target position, w 0 */
  target = (Actor *)cam->target;
  heading = target->base.rot[1];
  ahead[0] = __builtin_cosf(heading) * 10.0f;
  ahead[1] = 0.0f * 10.0f;
  ahead[2] = __builtin_sinf(heading) * 10.0f;
  ahead[3] = 0.0f;
  target = (Actor *)cam->target;
  ahead[0] = ahead[0] + target->base.pos[0];
  ahead[1] = ahead[1] + target->base.pos[1];
  ahead[2] = ahead[2] + target->base.pos[2];
  ahead[1] = ahead[1] + 12.0f;
  /* params->target = ahead * 10 (quest tables use 10x world units), w 0 */
  params = ((GameQuestCamParams *)cam->questCamParams)->target;
  params[0] = ahead[0] * 10.0f;
  params[1] = ahead[1] * 10.0f;
  params[2] = ahead[2] * 10.0f;
  params[3] = 0.0f;
  ctrl = (GameQuestCamCtrl *)cam->questCam;
  step = &ctrl->vtbl[2];
  ((void (*)(void *, float))step->fn)((u8 *)ctrl + step->delta, 0.033333335f);

  /* eye = ctrl->eye * 0.1, w 0 */
  ctrl = (GameQuestCamCtrl *)cam->questCam;
  cam->base.eye[0] = ctrl->eye.x * 0.1f;
  cam->base.eye[1] = ctrl->eye.y * 0.1f;
  cam->base.eye[2] = ctrl->eye.z * 0.1f;
  cam->base.eye[3] = 0.0f;
  /* look-at = ctrl->lookAt * 0.1, w 0; followLookAt = look-at */
  ctrl = (GameQuestCamCtrl *)cam->questCam;
  cam->base.target[0] = ctrl->lookAt.x * 0.1f;
  cam->base.target[1] = ctrl->lookAt.y * 0.1f;
  cam->base.target[2] = ctrl->lookAt.z * 0.1f;
  cam->base.target[3] = 0.0f;
  cam->followLookAt.x = cam->base.target[0];
  cam->followLookAt.y = cam->base.target[1];
  cam->followLookAt.z = cam->base.target[2];
  cam->followLookAt.w = cam->base.target[3];
  cam->yaw = atan2f(cam->base.target[2] - cam->base.eye[2], cam->base.target[0] - cam->base.eye[0]);
  ctrl = (GameQuestCamCtrl *)cam->questCam;
  cam->base.nearZ = ctrl->extra[0];
  cam->base.farZ = ctrl->extra[1];
}
