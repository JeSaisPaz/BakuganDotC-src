// bdc 0x08a18020 GmoSetViewMatrix
#include "bdc.h"

/* Installs the camera for GMO drawing: copies the 4x4 matrix `view` (or
   `g_gmoIdentityMatrix` when NULL) to `g_gmoViewMatrix` and the 4-float vector `eye`
   (or `g_gmoDefaultEyePos` when NULL) to `g_gmoEyePos`, then registers
   `GmoViewMaterialCallback` with `GmoSetMaterialCallback` (tail call). */

void GmoSetViewMatrix(const float *view, const float *eye)
{
  const float *src;
  float *dst;
  const float *end;

  /* Row-by-row word copy of the 16 matrix floats. */
  if (view == NULL) {
    src = &g_gmoIdentityMatrix.x.x;
  } else {
    src = view;
  }
  dst = &g_gmoViewMatrix.x.x;
  end = src + 16;
  do {
    float y = src[1];
    float z = src[2];
    float w = src[3];
    dst[0] = src[0];
    src += 4;
    dst[1] = y;
    dst[2] = z;
    dst[3] = w;
    dst += 4;
  } while (src != end);

  if (eye == NULL) {
    g_gmoEyePos.w = g_gmoDefaultEyePos.w;
    g_gmoEyePos.y = g_gmoDefaultEyePos.y;
    g_gmoEyePos.z = g_gmoDefaultEyePos.z;
    g_gmoEyePos.x = g_gmoDefaultEyePos.x;
  } else {
    g_gmoEyePos.w = eye[3];
    g_gmoEyePos.y = eye[1];
    g_gmoEyePos.z = eye[2];
    g_gmoEyePos.x = eye[0];
  }
  GmoSetMaterialCallback(GmoViewMaterialCallback);
}
