// bdc 0x089e327c GfxCameraUpdateShake
#include "bdc.h"

/* Advances a camera shake by one frame. `shakeFrames` counts towards 0 from either side (0 does
   nothing); each step the amplitude `shakeAmp` decays by `shakeDecay` (set to 0 when the count
   reaches 0), the vertical screen offset (`screenOffset[1]`, x kept) is set with
   `GfxCameraSetScreenOffset` to `amp * (sin(3*phase) + sin(phase - pi/2) + 1)` for a negative
   count or `amp * sin(phase)` for a positive one, and `shakePhase` then advances by `shakeSpeed`,
   wrapped by 2*pi when above pi (or NaN) or at most -pi. The sines are VFPU `vsin.s` of the angle
   times the bank constant 2/pi (S703), i.e. sines of the angle in radians. */

void GfxCameraUpdateShake(GfxCamera *cam)
{
  s32 frames;
  float phase;
  float x;
  float s1;
  float s2;

  frames = cam->shakeFrames;
  if (frames < 0) {
    frames = frames + 1;
    cam->shakeFrames = frames;
    phase = cam->shakePhase;
    x = cam->screenOffset[0];
    cam->shakeAmp = cam->shakeAmp - cam->shakeDecay;
    if (frames == 0) {
      cam->shakeAmp = 0.0f;
    }
    s1 = __builtin_sinf(phase * 3.0f);
    s2 = __builtin_sinf(phase - 1.57079637f);
    GfxCameraSetScreenOffset(x, (s1 + s2 + 1.0f) * cam->shakeAmp, cam);
  } else if (frames > 0) {
    frames = frames - 1;
    cam->shakeFrames = frames;
    phase = cam->shakePhase;
    x = cam->screenOffset[0];
    cam->shakeAmp = cam->shakeAmp - cam->shakeDecay;
    if (frames == 0) {
      cam->shakeAmp = 0.0f;
    }
    s1 = __builtin_sinf(phase);
    GfxCameraSetScreenOffset(x, s1 * cam->shakeAmp, cam);
  } else {
    return;
  }
  phase = cam->shakePhase + cam->shakeSpeed;
  cam->shakePhase = phase;
  if (!(phase <= 3.14159274f)) {
    cam->shakePhase = phase - 6.28318548f;
  } else if (phase <= -3.14159274f) {
    cam->shakePhase = phase + 6.28318548f;
  }
}
