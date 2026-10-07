// bdc 0x0897d75c UiCollectionSphereResetDetailPose
#include "bdc.h"

/* Resets the detail-model pose of `UiCollectionSphere` (task 312): zeroes
   `tilt`/`yaw`/`idleTimer`/`zoom`, sets `yaw` = 3.14 and `zoom` = 1, sets the root matrix of
   `models[detailCell]` to the Y rotation by 3.14, then multiplies it (VFPU `vmmul.q E200, E100,
   E000`) with the X rotation by 0, then scales fields 0-2 uniformly by
   `UiCollectionSphereGetDetailScale` × 1.4 (`withOffset` false) or × the second value from
   `UiCollectionSphereGetDetailOffset` (`withOffset` true); that scale is stored in
   `modelScale[detailCell]`. The VFPU angle factor S703 (2/π) cancels the quarter turns of `vrot`. */

void UiCollectionSphereResetDetailPose(UiCollectionSphere *self, bool withOffset)

{
  float rot[16];
  float tmp[16];
  float offset[2];
  float angle;
  float c;
  float s;
  float scale;
  float *matrix;
  int i;
  int j;

  memset(&self->tilt, 0, 0x10);
  self->yaw = 3.14f;
  self->zoom = 1.0f;
  matrix = self->models[self->detailCell]->data->rootMatrix;
  /* Y rotation: fields (c,0,-s,0), (0,1,0,0), (s,0,c,0), (0,0,0,1). */
  angle = 3.14f;
  c = __builtin_cosf(angle);
  s = __builtin_sinf(angle);
  matrix[0] = c;
  matrix[1] = 0.0f;
  matrix[2] = -s;
  matrix[3] = 0.0f;
  matrix[4] = 0.0f;
  matrix[5] = 1.0f;
  matrix[6] = 0.0f;
  matrix[7] = 0.0f;
  matrix[8] = s;
  matrix[9] = 0.0f;
  matrix[10] = c;
  matrix[11] = 0.0f;
  matrix[12] = 0.0f;
  matrix[13] = 0.0f;
  matrix[14] = 0.0f;
  matrix[15] = 1.0f;
  matrix = self->models[self->detailCell]->data->rootMatrix;
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
  /* vmmul.q E200, E100 (matrix), E000 (rot): field j lane i = sum_k matrix[k][i] * rot[k][j]. */
  for (j = 0; j < 4; j++) {
    for (i = 0; i < 4; i++) {
      tmp[j * 4 + i] = matrix[0 * 4 + i] * rot[0 * 4 + j] + matrix[1 * 4 + i] * rot[1 * 4 + j] +
                       matrix[2 * 4 + i] * rot[2 * 4 + j] + matrix[3 * 4 + i] * rot[3 * 4 + j];
    }
  }
  for (i = 0; i < 16; i++) {
    matrix[i] = tmp[i];
  }
  if (!withOffset) {
    scale = UiCollectionSphereGetDetailScale(self, self->detailCell) * 1.4f;
  } else {
    UiCollectionSphereGetDetailOffset(offset, &self->base);
    scale = UiCollectionSphereGetDetailScale(self, self->detailCell) * offset[1];
  }
  self->modelScale[self->detailCell] = scale;
  matrix = self->models[self->detailCell]->data->rootMatrix;
  /* vscl.q of fields 0-2 by (scale, scale, scale). */
  for (i = 0; i < 12; i++) {
    matrix[i] = matrix[i] * scale;
  }
}
