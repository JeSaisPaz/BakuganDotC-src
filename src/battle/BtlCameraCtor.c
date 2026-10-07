// bdc 0x08846fb8 BtlCameraCtor
#include "bdc.h"

/* Constructor of the battle camera controller embedded at `+0x20` of the battle main task
   (`BtlMainTaskCtor`): runs the base camera ctor (`GfxCameraCtor`), installs
   `g_btlCameraVtbl`, resets the defaults (`GfxCameraInit`), then sets eye (0, 60, 90),
   target (0, 0, 20), near 30, far 35000, fov 50 and rebuilds the matrices (`GfxCameraUpdate`,
   all flags). Zeroes `listenerPos` (VFPU bank constant C720 = 0) and binds the 3D sound
   listener's position and target to it (`SndListenerBind`); clears mode/target/follow
   fields, zeroes the cached vectors
   `lookPoint`, `focusPoint`, `lookTarget`, `prevFocusPoint`, `reservedVec360/370`, and sets the
   follow defaults (distance 450, fov 40) and lock-on defaults (mid blend 1, elevation and pitch
   0.1*pi, base pitch 0.2268928). Returns `camera`. */
BtlCamera *BtlCameraCtor(BtlCamera *camera)
{
  GfxCameraCtor((CoreNode *)camera);
  camera->base.base.vtable = g_btlCameraVtbl;
  GfxCameraInit(&camera->base);
  camera->base.eye[0] = 0.0f;
  camera->base.eye[1] = 60.0f;
  camera->base.eye[2] = 90.0f;
  camera->base.eye[3] = 0.0f;
  camera->base.target[0] = 0.0f;
  camera->base.target[1] = 0.0f;
  camera->base.target[2] = 20.0f;
  camera->base.target[3] = 0.0f;
  camera->base.nearZ = 30.0f;
  camera->base.farZ = 35000.0f;
  camera->base.fov = 50.0f;
  GfxCameraUpdate(&camera->base, 0xffffffffu);
  camera->listenerPos[0] = 0.0f;
  camera->listenerPos[1] = 0.0f;
  camera->listenerPos[2] = 0.0f;
  camera->listenerPos[3] = 0.0f;
  SndListenerBind(SndGetListener(), camera->listenerPos, camera->listenerPos);
  camera->mode = 0;
  camera->target = NULL;
  camera->followTarget = NULL;
  camera->lookPoint[0] = 0.0f;
  camera->lookPoint[1] = 0.0f;
  camera->lookPoint[2] = 0.0f;
  camera->lookPoint[3] = 0.0f;
  camera->focusPoint[0] = 0.0f;
  camera->focusPoint[1] = 0.0f;
  camera->focusPoint[2] = 0.0f;
  camera->focusPoint[3] = 0.0f;
  camera->lookTarget[0] = 0.0f;
  camera->lookTarget[1] = 0.0f;
  camera->lookTarget[2] = 0.0f;
  camera->lookTarget[3] = 0.0f;
  camera->prevFocusPoint[0] = 0.0f;
  camera->prevFocusPoint[1] = 0.0f;
  camera->prevFocusPoint[2] = 0.0f;
  camera->prevFocusPoint[3] = 0.0f;
  camera->turnFrames = 0;
  camera->yawOffset = 0.0f;
  camera->yaw = 0.0f;
  camera->kindParam3 = 85.0f;
  camera->lockOnHoldBlend = 0.0f;
  camera->lockOnPullBack = 0.0f;
  camera->lockOnSide = 0.0f;
  camera->lockOnFoe = NULL;
  camera->lockOnFlag = 0;
  camera->closeUpFrames = 0;
  camera->closeUpBlendA = 0.0f;
  camera->lockOnHoldFrames = 0;
  camera->reserved354 = 0;
  camera->reserved358 = 0.0f;
  camera->reservedVec360[0] = 0.0f;
  camera->reservedVec360[1] = 0.0f;
  camera->reservedVec360[2] = 0.0f;
  camera->reservedVec360[3] = 0.0f;
  camera->reservedVec370[0] = 0.0f;
  camera->reservedVec370[1] = 0.0f;
  camera->reservedVec370[2] = 0.0f;
  camera->reservedVec370[3] = 0.0f;
  camera->followTarget = NULL;
  camera->followDistance = 450.0f;
  camera->reserved388 = 0.0f;
  camera->followHeight = 0.0f;
  camera->reserved394 = 0.0f;
  camera->followFov = 40.0f;
  camera->reservedFov39c = 40.0f;
  camera->lockOnMidBlend = 1.0f;
  camera->followPos = NULL;
  camera->followYawSrc = NULL;
  camera->followDistanceOffset = 0.0f;
  camera->followOffsetY = 0.0f;
  camera->followOffset3f4 = 0.0f;
  camera->lockOnElevation = 0.314159274f;
  camera->zoomPulse = 0.0f;
  camera->lockOnRelax = 0.0f;
  camera->basePitch = 0.226892799f;
  camera->lockOnPitch = 0.314159274f;
  return camera;
}
