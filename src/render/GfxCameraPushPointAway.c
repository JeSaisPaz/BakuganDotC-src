// bdc 0x089bf0bc GfxCameraPushPointAway
#include "bdc.h"

/* Moves `pos` `dist` units further away from the camera (`g_gfxActiveCamera->eye`)
   along the camera→point direction and stores the result in the global vector
   `g_gfxPushedPoint`, which it also returns. The `w` lane stored is the bank
   constant S713 (0). */

float *GfxCameraPushPointAway(float dist, float *pos)
{
  float *eye = g_gfxActiveCamera->eye;
  float dx = pos[0] - eye[0];
  float dy = pos[1] - eye[1];
  float dz = pos[2] - eye[2];
  float k = VfRsq(dx * dx + dy * dy + dz * dz) * dist;

  g_gfxPushedPoint[0] = dx * k + pos[0];
  g_gfxPushedPoint[1] = dy * k + pos[1];
  g_gfxPushedPoint[2] = dz * k + pos[2];
  g_gfxPushedPoint[3] = 0.0f;
  return g_gfxPushedPoint;
}
