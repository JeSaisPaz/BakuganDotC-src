// bdc 0x089e3190 GfxScreenCameraSetOrtho
#include "bdc.h"

/* Builds an orthographic projection for the screen camera (`g_gfxScreenCamera->proj`) from a
   rectangle `{x0, x1, y0, y1, z0, z1}`: scale `-2/(x0-x1)` etc., translation `(x0+x1)/(x0-x1)`
   etc. (the VFPU `vbfy1`/`vrcp` butterfly). The VFPU scratch registers it leaves behind are not
   read by its only caller, `GfxScreenCameraCreate`. */

void GfxScreenCameraSetOrtho(const float *rect)
{
  ScePspFMatrix4 *m = &g_gfxScreenCamera->proj;
  float rx = VfRcp(VfSub(rect[0], rect[1]));
  float ry = VfRcp(VfSub(rect[2], rect[3]));
  float rz = VfRcp(VfSub(rect[4], rect[5]));

  m->x.x = VfMul(-2.0f, rx);
  m->x.y = 0.0f;
  m->x.z = 0.0f;
  m->x.w = 0.0f;
  m->y.x = 0.0f;
  m->y.y = VfMul(-2.0f, ry);
  m->y.z = 0.0f;
  m->y.w = 0.0f;
  m->z.x = 0.0f;
  m->z.y = 0.0f;
  m->z.z = VfMul(2.0f, rz);
  m->z.w = 0.0f;
  m->w.x = VfMul(VfAdd(rect[0], rect[1]), rx);
  m->w.y = VfMul(VfAdd(rect[2], rect[3]), ry);
  m->w.z = VfMul(VfAdd(rect[4], rect[5]), rz);
  m->w.w = 1.0f;
}
