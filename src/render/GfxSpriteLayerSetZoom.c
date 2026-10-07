// bdc 0x089f59b8 GfxSpriteLayerSetZoom
#include "bdc.h"

/* Zooms and rotates a whole sprite layer about a pivot: rebuilds its view matrix as identity with
   the translation `w.xyz = 0 - pivot * scale`, multiplies it on the left by a Z rotation of `angle`
   (rows (c, -s), (s, c); the VFPU `vrot` takes quarter turns, so `angle * 2/pi`), scales the x/y/z
   columns (all four lanes) by `scale` and adds `pivot` back to `w.xyz`: view = pivot + scale *
   R * (v - pivot). A NULL `pivot` (only x/y/z are read) defaults to the screen centre
   `g_gfxLayerZoomDefaultPivot` (240, 136, 0), lazily initialised on the first call (guard
   `g_gfxLayerZoomPivotInit`). */

void GfxSpriteLayerSetZoom(float scale, float angle, GfxSpriteLayer *self, const float *pivot)
{
    ScePspFMatrix4 *m = &self->view;
    float t;
    float c;
    float s;
    float tx;
    float ty;
    float tz;

    if (g_gfxLayerZoomPivotInit == 0) {
        g_gfxLayerZoomPivotInit = 1;
        g_gfxLayerZoomDefaultPivot[0] = 240.0f;
        g_gfxLayerZoomDefaultPivot[2] = 0.0f;
        g_gfxLayerZoomDefaultPivot[1] = 136.0f;
        g_gfxLayerZoomDefaultPivot[3] = 0.0f;
    }
    if (pivot == NULL) {
        pivot = g_gfxLayerZoomDefaultPivot;
    }

    /* identity translated by -(pivot * scale) */
    tx = 0.0f - pivot[0] * scale;
    ty = 0.0f - pivot[1] * scale;
    tz = 0.0f - pivot[2] * scale;

    /* rotZ (S703 = 2/pi: quarter turns) times that matrix */
    t = angle * 0.636619747f;
    c = VfCosQuarter(t);
    s = VfSinQuarter(t);

    /* columns x/y/z scaled by `scale`, then pivot added back to w.xyz */
    m->x.x = c * scale;
    m->x.y = s * scale;
    m->x.z = 0.0f * scale;
    m->x.w = 0.0f * scale;
    m->y.x = -s * scale;
    m->y.y = c * scale;
    m->y.z = 0.0f * scale;
    m->y.w = 0.0f * scale;
    m->z.x = 0.0f * scale;
    m->z.y = 0.0f * scale;
    m->z.z = 1.0f * scale;
    m->z.w = 0.0f * scale;
    m->w.x = (c * tx + -s * ty) + pivot[0];
    m->w.y = (s * tx + c * ty) + pivot[1];
    m->w.z = tz + pivot[2];
    m->w.w = 1.0f;
}
