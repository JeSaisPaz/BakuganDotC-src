// bdc 0x089e3848 GfxCameraUpdateYaw
#include "bdc.h"

/* Updates and returns the camera's yaw `yaw`: `-atan2(dir.z, dir.x)` of the view direction
   `dir` (scaled by 10, only when the squared XZ length of the scaled vector is above 1), wrapped
   to (-pi, pi]. Otherwise returns the previous yaw unchanged. Called by `GfxCameraUpdate`. */

float GfxCameraUpdateYaw(GfxCamera *cam)
{
  float x = cam->dir[0] * 10.0f;
  float z = cam->dir[2] * 10.0f;
  float y = 0.0f;
  float a;

  if (!(x * x + y * y + z * z <= 1.0f)) {
    a = -atan2f(z, x);
    cam->yaw = a;
    if (!(a <= 3.1415927f)) {
      cam->yaw = cam->yaw - 6.2831855f;
    } else if (cam->yaw <= -3.1415927f) {
      cam->yaw = cam->yaw + 6.2831855f;
    }
  }
  return cam->yaw;
}
