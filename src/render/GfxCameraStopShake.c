// bdc 0x089e348c GfxCameraStopShake
#include "bdc.h"

/* Clears a camera's shake state (`+0x210..+0x220`) and resets its screen offset
   (`GfxCameraSetScreenOffset` 0, 0). */

void GfxCameraStopShake(GfxCamera *cam)

{
  cam->shakeFrames = 0;
  cam->shakeAmp = 0.0f;
  cam->shakeSpeed = 0.0f;
  cam->shakePhase = 0.0f;
  cam->shakeDecay = 0.0f;
  GfxCameraSetScreenOffset(0.0f, 0.0f, cam);
  return;
}

