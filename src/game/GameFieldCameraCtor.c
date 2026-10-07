// bdc 0x088b9820 GameFieldCameraCtor
#include "bdc.h"

/* Constructor of the field camera object embedded in the field task (`GameFieldCtor`): runs the
   base camera constructor `GfxCameraCtor`, installs the vtable `g_gameFieldCameraVtbl` (slot 1
   `GameFieldCameraDtor`, slot 2 `GameFieldCameraUpdate`), builds the mode helpers `mode9`
   (`GameFieldCameraMode9Ctor`), `headView` (`GameFieldCameraHeadViewCtor`) and `aimView`
   (`GameFieldCameraAimViewCtor`), resets the camera (`GfxCameraInit`) and sets fov 50,
   eye (0, 60, 90), look-at (0, 0, 20), near 3.9 and far 30000, then rebuilds every matrix
   (`GfxCameraUpdate` with all flags). Binds the 3D sound listener to `listenerPos`
   (`SndListenerBind`) and clears the follow state: mode fields and target 0, yaw 0, pitch
   0.1222, distance `g_gameFieldCameraDefaultDistance`, quest camera controller and params NULL,
   shake state 0. Returns `cam`.
   `listenerPos`, `followLookAt`, `followGoal`, `ctorVec450` and `shakeOffset` are zeroed from the
   VFPU constant bank's zero vector C720. */

void *GameFieldCameraCtor(GameFieldCamera *cam)
{
  SndListener *listener;

  GfxCameraCtor((CoreNode *)cam);
  cam->base.base.vtable = g_gameFieldCameraVtbl;
  GameFieldCameraMode9Ctor((GameFieldCameraMode9State **)cam->mode9);
  GameFieldCameraHeadViewCtor(cam->headView);
  GameFieldCameraAimViewCtor(cam->aimView);
  GfxCameraInit(&cam->base);
  cam->base.fov = 50.0f;
  cam->base.eye[0] = 0.0f;
  cam->base.eye[1] = 60.0f;
  cam->base.eye[2] = 90.0f;
  cam->base.eye[3] = 0.0f;
  cam->base.target[0] = 0.0f;
  cam->base.target[1] = 0.0f;
  cam->base.target[2] = 20.0f;
  cam->base.target[3] = 0.0f;
  cam->base.nearZ = 3.9f;
  cam->base.farZ = 30000.0f;
  GfxCameraUpdate(&cam->base, 0xffffffffu);
  cam->listenerPos.x = 0.0f;
  cam->listenerPos.y = 0.0f;
  cam->listenerPos.z = 0.0f;
  cam->listenerPos.w = 0.0f;
  listener = SndGetListener();
  SndListenerBind(listener, &cam->listenerPos.x, &cam->listenerPos.x);
  cam->mode = 0;
  cam->prevMode = 0;
  cam->lastMode = 0;
  cam->target = NULL;
  cam->followLookAt.x = 0.0f;
  cam->followLookAt.y = 0.0f;
  cam->followLookAt.z = 0.0f;
  cam->followLookAt.w = 0.0f;
  cam->followGoal.x = 0.0f;
  cam->followGoal.y = 0.0f;
  cam->followGoal.z = 0.0f;
  cam->followGoal.w = 0.0f;
  cam->yawVel = 0.0f;
  cam->yaw = 0.0f;
  cam->pitch = 0.122173049f;
  cam->distance = g_gameFieldCameraDefaultDistance;
  cam->wallAvoidTimer = 0;
  cam->talkBlend = 0;
  cam->stepState = 0;
  cam->ctorByte3b9 = 0;
  cam->ctorWord3b0 = 0;
  cam->ctorWord3b4 = 0;
  cam->ctorByte3b8 = 0xff;
  cam->orbitPitch = 0.0f;
  cam->questCam = NULL;
  cam->questCamParams = NULL;
  cam->ctorVec450.x = 0.0f;
  cam->ctorVec450.y = 0.0f;
  cam->ctorVec450.z = 0.0f;
  cam->ctorVec450.w = 0.0f;
  cam->ctorRate464 = 0.5f;
  cam->ctorFlag460 = 0;
  cam->ctorFloat46c = 0.0f;
  cam->ctorFloat470 = 0.0f;
  cam->shakeOffset.x = 0.0f;
  cam->shakeOffset.y = 0.0f;
  cam->shakeOffset.z = 0.0f;
  cam->shakeOffset.w = 0.0f;
  cam->shakeAngle = 0.0f;
  cam->shakeRight = 0.0f;
  cam->shakeUp = 0.0f;
  cam->shakeAmplitude = 0.0f;
  cam->shakeTimer = 0;
  return cam;
}
