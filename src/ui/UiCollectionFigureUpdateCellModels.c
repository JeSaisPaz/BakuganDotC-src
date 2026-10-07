// bdc 0x0898d430 UiCollectionFigureUpdateCellModels
#include "bdc.h"

/* Advances the zoom of the six cell models of the figure collection screen by one frame: each
   loaded cell's tween time `cellTweens[i].t` grows by 1/16 (1/8 while `pageDir` is set). Zoom in
   (`out` false): alpha = startAlpha + (1 - (t - 1)^2) and scale = startScale - (1 - (t - 1)^2) *
   (0.12 - baseScale); once t >= 1 (or NaN) the cell is snapped to alpha 1 and `baseScale`. Zoom
   out: alpha = startAlpha - t^2 and scale = startScale + t^2 * (0.12 - baseScale); a finished
   cell gets alpha 0. Each step rebuilds the model's root matrix as the Y rotation by 3.14
   multiplied (VFPU `vmmul.q E200, E100, E000`) with the X rotation by -0, then scales its fields
   0-2 by the scale, which is also stored in `cellScale[i]`. The VFPU angle factor S703 (2/π)
   cancels the quarter turns of `vrot`. Returns true when no cell holds a model; otherwise returns
   doneCount != 0, i.e. true when at least one cell finished this frame, not only when all did. */

#define CELL_COUNT 6

/* Writes the Y rotation by `yaw` into `m`, then replaces it with the `vmmul.q E200, E100, E000`
   product of it and the X rotation by `pitch`: field j lane i = sum_k m[k][j] * rot[k][i]. */
static inline void CellModelSetRotation(float *m, float yaw, float pitch)
{
    float rot[16];
    float tmp[16];
    float c;
    float s;
    int i;
    int j;

    /* Y rotation: fields (c,0,-s,0), (0,1,0,0), (s,0,c,0), (0,0,0,1). */
    c = __builtin_cosf(yaw);
    s = __builtin_sinf(yaw);
    m[0] = c;
    m[1] = 0.0f;
    m[2] = -s;
    m[3] = 0.0f;
    m[4] = 0.0f;
    m[5] = 1.0f;
    m[6] = 0.0f;
    m[7] = 0.0f;
    m[8] = s;
    m[9] = 0.0f;
    m[10] = c;
    m[11] = 0.0f;
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 1.0f;
    /* X rotation: fields (1,0,0,0), (0,c,s,0), (0,-s,c,0), (0,0,0,1). */
    c = __builtin_cosf(pitch);
    s = __builtin_sinf(pitch);
    rot[0] = 1.0f;
    rot[1] = 0.0f;
    rot[2] = 0.0f;
    rot[3] = 0.0f;
    rot[4] = 0.0f;
    rot[5] = c;
    rot[6] = s;
    rot[7] = 0.0f;
    rot[8] = 0.0f;
    rot[9] = -s;
    rot[10] = c;
    rot[11] = 0.0f;
    rot[12] = 0.0f;
    rot[13] = 0.0f;
    rot[14] = 0.0f;
    rot[15] = 1.0f;
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            tmp[j * 4 + i] = m[0 * 4 + j] * rot[0 * 4 + i] + m[1 * 4 + j] * rot[1 * 4 + i] +
                             m[2 * 4 + j] * rot[2 * 4 + i] + m[3 * 4 + j] * rot[3 * 4 + i];
        }
    }
    for (i = 0; i < 16; i++) {
        m[i] = tmp[i];
    }
}

/* vscl.q of fields 0-2 (all four lanes) by (scale, scale, scale). */
static inline void CellModelScale(float *m, float scale)
{
    int i;

    for (i = 0; i < 12; i++) {
        m[i] = m[i] * scale;
    }
}

bool UiCollectionFigureUpdateCellModels(UiCollectionFigure *self, bool out)
{
    const float yaw = 3.14f;     /* 0x4048f5c3 */
    const float pitch = -0.0f;   /* 0x80000000 */
    float frames;
    float base;
    float zoom, ease, scale;
    u8 cellCount;
    u8 doneCount;
    s32 i;

    doneCount = 0;
    frames = self->pageDir != 0 ? 8.0f : 16.0f;

    cellCount = 0;
    for (i = 0; i < CELL_COUNT; i++) {
        if (self->models[i] != NULL) {
            cellCount++;
        }
    }
    if (cellCount == 0) {
        return 1;
    }

    if (!out) {
        /* Zoom in: alpha and scale follow the ease-out curve 1 - (t - 1)^2. */
        for (i = 0; i < CELL_COUNT; i++) {
            if (self->models[i] == NULL) {
                continue;
            }
            base = self->baseScale;
            zoom = 0.12f - base;
            self->cellTweens[i].t += 1.0f / frames;
            ease = self->cellTweens[i].t - 1.0f;
            self->models[i]->ambient[3] = self->cellTweens[i].startAlpha + (1.0f - ease * ease);
            CellModelSetRotation(self->models[i]->data->rootMatrix, yaw, pitch);
            ease = self->cellTweens[i].t - 1.0f;
            scale = self->cellTweens[i].startScale - (1.0f - ease * ease) * zoom;
            self->cellScale[i] = scale;
            CellModelScale(self->models[i]->data->rootMatrix, scale);
            if (!(self->cellTweens[i].t < 1.0f)) {
                self->models[i]->ambient[3] = 1.0f;
                CellModelSetRotation(self->models[i]->data->rootMatrix, yaw, pitch);
                self->cellScale[i] = base;
                CellModelScale(self->models[i]->data->rootMatrix, base);
                doneCount++;
            }
        }
        return doneCount != 0;
    }

    /* Zoom out: alpha falls and scale grows with t^2. */
    for (i = 0; i < CELL_COUNT; i++) {
        if (self->models[i] == NULL) {
            continue;
        }
        zoom = 0.12f - self->baseScale;
        self->cellTweens[i].t += 1.0f / frames;
        self->models[i]->ambient[3] =
            self->cellTweens[i].startAlpha - self->cellTweens[i].t * self->cellTweens[i].t;
        CellModelSetRotation(self->models[i]->data->rootMatrix, yaw, pitch);
        scale = self->cellTweens[i].startScale + self->cellTweens[i].t * self->cellTweens[i].t * zoom;
        self->cellScale[i] = scale;
        CellModelScale(self->models[i]->data->rootMatrix, scale);
        if (!(self->cellTweens[i].t < 1.0f)) {
            doneCount++;
            self->models[i]->ambient[3] = 0.0f;
        }
    }
    return doneCount != 0;
}
