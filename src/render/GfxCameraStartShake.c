// bdc 0x089e3230 GfxCameraStartShake
#include "bdc.h"

/* Starts a camera shake: `frames` (`+0x210`, at least 1; the sign selects the axis), amplitude
   `amp` (`+0x214`) decaying by `amp/|frames|` per frame (`+0x220`), phase speed `speed` (`+0x218`)
   and the start phase (`+0x21c`, 0 or pi/2). Run by `GfxCameraUpdateShake`. */

void GfxCameraStartShake(float amp, float speed, GfxCamera *cam, s32 frames)

{
  if (frames == 0) {
    frames = 1;
  }
  cam->shakeFrames = frames;
  cam->shakeAmp = amp;
  cam->shakeSpeed = speed;
  cam->shakeDecay = amp / ABS((float)frames);
  if (frames < 0) {
    cam->shakePhase = 0.0f;
    return;
  }
  cam->shakePhase = 1.5707964f;
  return;
}

