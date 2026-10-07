// bdc 0x089bee3c GfxProjectToScreen
#include "bdc.h"

/* Projects the 3D point `pos` with the 4x4 matrix `mtx` (VFPU `vtfm4`, pos.w = 1), divides by `w` and
   maps the result to PSP screen pixels: `out[0] = (x*480 + 480)/2`, `out[1] = (-y*272 + 272)/2`,
   `out[2]` = projected z/w, `out[3] = 1/w`. Returns the clip-space vector `mtx * (pos, 1)` (VFPU C100,
   read by the callers of `GfxCameraProjectPoint`). */

ScePspFVector4 GfxProjectToScreen(float *out, ScePspFMatrix4 *mtx, float *pos)
{
    ScePspFVector4 clip;
    float rw;

    clip.x = mtx->x.x * pos[0] + mtx->y.x * pos[1] + mtx->z.x * pos[2] + mtx->w.x * 1.0f;
    clip.y = mtx->x.y * pos[0] + mtx->y.y * pos[1] + mtx->z.y * pos[2] + mtx->w.y * 1.0f;
    clip.z = mtx->x.z * pos[0] + mtx->y.z * pos[1] + mtx->z.z * pos[2] + mtx->w.z * 1.0f;
    clip.w = mtx->x.w * pos[0] + mtx->y.w * pos[1] + mtx->z.w * pos[2] + mtx->w.w * 1.0f;
    rw = 1.0f / clip.w;
    out[0] = clip.x * rw;
    out[1] = clip.y * rw;
    out[2] = clip.z * rw;
    out[3] = rw;
    out[0] = (out[0] * 480.0f + 480.0f) * 0.5f;
    out[1] = (out[1] * -272.0f + 272.0f) * 0.5f;
    return clip;
}
