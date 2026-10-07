// bdc 0x0898efb8 UiCollectionFigureResetDetailPose
#include "bdc.h"

/* Resets the pose of the detail model (`models[detailCell]`) of the figure collection screen
   (task 314, `maybe_UiScreen314Ctor`): when `keep` is 0 clears `pitch`/`yaw`/`idleTimer` and sets
   `zoom` to 1; then sets `yaw` to 3.14, rebuilds the root matrix as the Y rotation by 3.14, multiplied
   (VFPU `vmmul.q E200, E100, E000`) with the X rotation by 0, and scales its fields 0-2 by
   `baseScale` × `UiCollectionFigureGetDetailScale` of the selected entry (view 0, or view 1 times
   `zoom` when `keep`), also stored in `cellScale[detailCell]`. The VFPU angle factor S703 (2/π)
   cancels the quarter turns of `vrot`. */

void UiCollectionFigureResetDetailPose(UiCollectionFigure *self, bool keep)
{
  float rot[16];
  float tmp[16];
  float angle;
  float c;
  float s;
  float detail;
  float scale;
  float *m;
  int i;
  int j;

  if (!keep) {
    memset(&self->pitch, 0, 0x10);
    self->zoom = 1.0f;
  }
  self->yaw = 3.14f;
  m = self->models[self->detailCell]->data->rootMatrix;
  /* Y rotation: fields (c,0,-s,0), (0,1,0,0), (s,0,c,0), (0,0,0,1). */
  angle = 3.14f;
  c = __builtin_cosf(angle);
  s = __builtin_sinf(angle);
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
  m = self->models[self->detailCell]->data->rootMatrix;
  /* X rotation: fields (1,0,0,0), (0,c,s,0), (0,-s,c,0), (0,0,0,1). */
  angle = 0.0f;
  c = __builtin_cosf(angle);
  s = __builtin_sinf(angle);
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
  /* vmmul.q E200, E100 (m), E000 (rot): field j lane i = sum_k m[k][j] * rot[k][i]. */
  for (j = 0; j < 4; j++) {
    for (i = 0; i < 4; i++) {
      tmp[j * 4 + i] = m[0 * 4 + j] * rot[0 * 4 + i] + m[1 * 4 + j] * rot[1 * 4 + i] +
                       m[2 * 4 + j] * rot[2 * 4 + i] + m[3 * 4 + j] * rot[3 * 4 + i];
    }
  }
  for (i = 0; i < 16; i++) {
    m[i] = tmp[i];
  }
  if (!keep) {
    detail = UiCollectionFigureGetDetailScale(self, 0,
                                              self->entryIds[self->cursor + self->page * 6]);
    scale = self->baseScale * detail;
  } else {
    detail = UiCollectionFigureGetDetailScale(self, 1,
                                              self->entryIds[self->cursor + self->page * 6]);
    scale = self->baseScale * detail * self->zoom;
  }
  self->cellScale[self->detailCell] = scale;
  m = self->models[self->detailCell]->data->rootMatrix;
  /* vscl.q of fields 0-2 by (scale, scale, scale). */
  for (i = 0; i < 12; i++) {
    m[i] = m[i] * scale;
  }
}
