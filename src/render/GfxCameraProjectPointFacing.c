// bdc 0x089e34c0 GfxCameraProjectPointFacing
#include "bdc.h"

/* Projects the world point `pos` to screen space (`GfxCameraProjectPoint` logic: cached
   view-projection, `GfxProjectToScreen`) and stores in `out[2]` the cosine between the camera direction
   `dir` and the direction from the point to the eye `eye`, i.e. how directly the point faces the
   camera. A zero-length point→eye vector uses a scale of 0 (bank constant S713), giving 0. The
   normalised components are clamped to [-1, 1]. */

void GfxCameraProjectPointFacing(GfxCamera *cam, float *out, const float *pos)
{
  float dx;
  float dy;
  float dz;
  float len2;
  float s;
  float nx;
  float ny;
  float nz;

  GfxCameraUpdateViewProj(cam);
  GfxProjectToScreen(out, &cam->viewProj, (float *)pos);
  dx = cam->eye[0] - pos[0];
  dy = cam->eye[1] - pos[1];
  dz = cam->eye[2] - pos[2];
  len2 = dx * dx + dy * dy + dz * dz;
  if (len2 == 0.0f) {
    s = 0.0f;
  } else {
    s = VfRsq(len2);
  }
  nx = VfSat1(dx * s);
  ny = VfSat1(dy * s);
  nz = VfSat1(dz * s);
  out[2] = cam->dir[0] * nx + cam->dir[1] * ny + cam->dir[2] * nz;
}
