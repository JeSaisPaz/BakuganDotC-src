// bdc 0x089e35c8 GfxCameraUpdateViewProj
#include "bdc.h"

/* Column `col` of `proj * view`: the sum over k of col[k] * column k of `proj`. */
static void GfxCameraViewProjColumn(ScePspFVector4 *d, const ScePspFMatrix4 *a, const ScePspFVector4 *col)
{
  d->x = col->x * a->x.x + col->y * a->y.x + col->z * a->z.x + col->w * a->w.x;
  d->y = col->x * a->x.y + col->y * a->y.y + col->z * a->z.y + col->w * a->w.y;
  d->z = col->x * a->x.z + col->y * a->y.z + col->z * a->z.z + col->w * a->w.z;
  d->w = col->x * a->x.w + col->y * a->y.w + col->z * a->z.w + col->w * a->w.w;
}

/* Computes the camera's combined projection x view matrix into `viewProj` once per update (guarded
   by the byte `viewProjValid`, cleared when the camera changes): viewProj = proj * view
   (`vmmul.q M000, M100, M200` with proj in M100, view in M200, column-major). */

void GfxCameraUpdateViewProj(GfxCamera *cam)
{
  if (cam->viewProjValid == 0) {
    cam->viewProjValid = 1;
    GfxCameraViewProjColumn(&cam->viewProj.x, &cam->proj, &cam->view.x);
    GfxCameraViewProjColumn(&cam->viewProj.y, &cam->proj, &cam->view.y);
    GfxCameraViewProjColumn(&cam->viewProj.z, &cam->proj, &cam->view.z);
    GfxCameraViewProjColumn(&cam->viewProj.w, &cam->proj, &cam->view.w);
  }
}
