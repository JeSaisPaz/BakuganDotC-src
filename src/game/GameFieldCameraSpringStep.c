// bdc 0x088ccf80 GameFieldCameraSpringStep
#include "bdc.h"

/* Steps a camera spring holder (pointer to a `GameFieldCameraSpring`: look-at velocity `+0x00`,
   eye velocity `+0x10`, eye `+0x20`, look-at `+0x30`) towards the targets `targetEye`/`targetLook`
   with a damped spring (stiffness `(10 / (2*damping))²`, velocity damping 10, dt 1/30) and writes
   the new positions (all four lanes) to `outEye`/`outLook`. Only x/y/z of the velocities and
   positions change; their w lanes are kept. */

void GameFieldCameraSpringStep(float damping, void **holder, float *outEye, float *outLook, float *targetEye, float *targetLook)
{
  GameFieldCameraSpring *s = (GameFieldCameraSpring *)*holder;
  float twice;
  float k;
  float acc;
  int i;

  twice = damping * 2.0f;
  if (g_gameFieldCameraSpringOmegaInit == 0) {
    g_gameFieldCameraSpringOmegaInit = 1;
    g_gameFieldCameraSpringOmega = 10.0f;
  }
  k = g_gameFieldCameraSpringOmega / twice;
  k = k * k;
  /* accel = (targetEye - eye) * k - eyeVel * omega; eyeVel += accel / 30; eye += eyeVel / 30 */
  for (i = 0; i < 3; i++) {
    acc = VfMul(VfSub(targetEye[i], s->eye[i]), k);
    acc = VfSub(acc, VfMul(s->eyeVel[i], g_gameFieldCameraSpringOmega));
    acc = VfMul(acc, 0.0333333351f);
    s->eyeVel[i] = VfAdd(s->eyeVel[i], acc);
    s->eye[i] = VfAdd(s->eye[i], VfMul(s->eyeVel[i], 0.0333333351f));
  }
  if (g_gameFieldCameraSpringOmegaInit == 0) {
    g_gameFieldCameraSpringOmegaInit = 1;
    g_gameFieldCameraSpringOmega = 10.0f;
  }
  k = g_gameFieldCameraSpringOmega / twice;
  k = k * k;
  /* same step for the look-at: targetLook, look, lookVel */
  for (i = 0; i < 3; i++) {
    acc = VfMul(VfSub(targetLook[i], s->look[i]), k);
    acc = VfSub(acc, VfMul(s->lookVel[i], g_gameFieldCameraSpringOmega));
    acc = VfMul(acc, 0.0333333351f);
    s->lookVel[i] = VfAdd(s->lookVel[i], acc);
    s->look[i] = VfAdd(s->look[i], VfMul(s->lookVel[i], 0.0333333351f));
  }
  for (i = 0; i < 4; i++) {
    outEye[i] = s->eye[i];
  }
  for (i = 0; i < 4; i++) {
    outLook[i] = s->look[i];
  }
}
