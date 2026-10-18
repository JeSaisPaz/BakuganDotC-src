// bdc 0x089e35c8 GfxCameraUpdateViewProj
#include "bdc.h"

/* Column `col` of `proj * view`: the sum over k of col[k] * column k of `proj`. */
static void GfxCameraViewProjColumn(ScePspFVector4 *d, const ScePspFMatrix4 *a, const ScePspFVector4 *col)
{
  d->x = VfDot4(col->x, col->y, col->z, col->w, a->x.x, a->y.x, a->z.x, a->w.x);
  d->y = VfDot4(col->x, col->y, col->z, col->w, a->x.y, a->y.y, a->z.y, a->w.y);
  d->z = VfDot4(col->x, col->y, col->z, col->w, a->x.z, a->y.z, a->z.z, a->w.z);
  d->w = VfDot4(col->x, col->y, col->z, col->w, a->x.w, a->y.w, a->z.w, a->w.w);
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
