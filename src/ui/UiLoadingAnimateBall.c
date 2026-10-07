// bdc 0x0890beac UiLoadingAnimateBall
#include "bdc.h"

/* Animates the loading ball of the now-loading screen (task 10100 / 0x2774, `UiLoadingCtor`):
   rocks shared sprite 21 of `g_uiLoadingShared` by the swinging angle `ballSwing` (rotation
   reduced by it each frame, swing grows by 0.01 clamped to ±0.25), fades that sprite in by 0.1 per
   frame up to 1, and pulses sprite 22's alpha as `0.8 + 0.15 * sin(ballPhase)` with `ballPhase`
   advancing 0.1 per frame and wrapping past 2*pi (the sine is `vsin.s` of `ballPhase` times the
   VFPU bank constant S703 = 2/pi, i.e. the sine of the angle in radians). */

#define UI_LOADING_TWO_PI 6.2831855f

void UiLoadingAnimateBall(UiLoading *self)

{
  GfxSprite *ball;
  GfxSprite *glow;
  float swing;
  float alpha;
  float phase;

  ball = g_uiLoadingShared->sprites[21];
  glow = g_uiLoadingShared->sprites[22];
  ball->maybe_billboardParams80[2] = ball->maybe_billboardParams80[2] - self->ballSwing;
  GfxSpriteSetScaleRotation(ball, ball->maybe_billboardParams80[0],
                            ball->maybe_billboardParams80[1],
                            ball->maybe_billboardParams80[2], false);
  swing = self->ballSwing + 0.01f;
  if (!(swing <= 0.25f)) {
    swing = 0.25f;
  }
  else if (swing < -0.25f) {
    swing = -0.25f;
  }
  self->ballSwing = swing;
  if (ball->alpha < 1.0f) {
    alpha = ball->alpha + 0.1f;
    if (!(alpha <= 1.0f)) {
      alpha = 1.0f;
    }
    ball->alpha = alpha;
  }
  phase = self->ballPhase + 0.1f;
  self->ballPhase = phase;
  if (!(phase <= UI_LOADING_TWO_PI)) {
    self->ballPhase = self->ballPhase - UI_LOADING_TWO_PI;
  }
  glow->alpha = __builtin_sinf(self->ballPhase) * 0.15f + 0.8f;
}
