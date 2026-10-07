// bdc 0x0897f904 UiCollectionSphereUpdateCellModels
#include "bdc.h"

/* Steps the fade/zoom of the cell models of `UiCollectionSphere`
   (task 312) by 1/16 (1/8 while paging, `pageDir` != 0) of `cells[i].moveT` per call. For every
   loaded `models[i]` it resets the root matrix to rotY(3.14) * rotX(`UiCollectionSphereGetModelTilt`) and scales rows 0-2 by the new `modelScale[i]`; the per-type amount is
   0.2 / 0.09 / 0.1 for cell types 0 / 1 / 2 (other types keep the previous cell's value).
   `out` false (fade in): alpha = alphaFrom + (1 - (t-1)^2), scale = scaleFrom - (1 - (t-1)^2) * amount;
   at t >= 1 alpha is set to 1 and the pose is rebuilt with scale 0.4 / 0.18 / 0.2.
   `out` true (fade out): alpha = alphaFrom - t^2, scale = scaleFrom + t^2 * amount; at t >= 1 alpha
   is set to 0. Returns true when no model is loaded or when at least one cell reached t >= 1.
   The VFPU `vrot` angles are scaled by the bank's 2/pi (S703), i.e. plain `cosf`/`sinf`. */

static inline void UiCollectionSphereCellPose(UiCollectionSphere *self, int i)
{
  float *m;
  float c;
  float s;
  float tilt;
  float y;
  float z;
  int r;

  /* rootMatrix = rotY(3.14f): vrot of 3.14f * S703 (2/pi) is cosf/sinf(3.14f). */
  m = self->models[i]->data->rootMatrix;
  c = __builtin_cosf(3.14f); /* 0x4048f5c3 */
  s = __builtin_sinf(3.14f);
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
  /* rootMatrix = rootMatrix * rotX(tilt): each row (x, y, z, w) becomes
     (x, c*y - s*z, s*y + c*z, w). */
  m = self->models[i]->data->rootMatrix;
  tilt = UiCollectionSphereGetModelTilt(self, (u8)i);
  c = __builtin_cosf(tilt);
  s = __builtin_sinf(tilt);
  for (r = 0; r < 4; r++) {
    y = m[r * 4 + 1];
    z = m[r * 4 + 2];
    m[r * 4 + 1] = c * y - s * z;
    m[r * 4 + 2] = s * y + c * z;
  }
}

static inline void UiCollectionSphereCellScale(UiCollectionSphere *self, int i, float scale)
{
  float *m;
  int r;

  self->modelScale[i] = scale;
  m = self->models[i]->data->rootMatrix;
  /* vscl.q of rows 0-2 (all four lanes) by the vector (scale, scale, scale, 0). */
  for (r = 0; r < 12; r++) {
    m[r] = m[r] * scale;
  }
}

bool UiCollectionSphereUpdateCellModels(UiCollectionSphere *self, bool out)

{
  UiCollectionSphereCell *cell;
  float frames;
  float amount;
  float endScale;
  float t;
  u8 loaded;
  u8 done;
  int i;

  amount = 0.0f;
  endScale = 0.0f;
  done = 0;
  if (self->pageDir != 0) {
    frames = 8.0f;
  } else {
    frames = 16.0f;
  }
  loaded = 0;
  for (i = 0; i < 7; i++) {
    if (self->models[i] != NULL) {
      loaded++;
    }
  }
  if (loaded == 0) {
    return true;
  }
  if (!out) {
    for (i = 0; i < 7; i++) {
      if (self->models[i] == NULL) {
        continue;
      }
      cell = &self->cells[i];
      if (cell->type == 0) {
        amount = 0.20000002f;
        endScale = 0.4f;
      } else if (cell->type < 2) {
        amount = 0.09f;
        endScale = 0.18f;
      } else if (cell->type < 3) {
        amount = 0.10000001f;
        endScale = 0.2f;
      }
      cell->moveT = cell->moveT + 1.0f / frames;
      t = cell->moveT - 1.0f;
      self->models[i]->ambient[3] = cell->alphaFrom + (1.0f - t * t);
      UiCollectionSphereCellPose(self, i);
      t = cell->moveT - 1.0f;
      UiCollectionSphereCellScale(self, i, cell->scaleFrom - (1.0f - t * t) * amount);
      if (!(cell->moveT < 1.0f)) {
        self->models[i]->ambient[3] = 1.0f;
        UiCollectionSphereCellPose(self, i);
        UiCollectionSphereCellScale(self, i, endScale);
        done++;
      }
    }
    return done != 0;
  }
  for (i = 0; i < 7; i++) {
    if (self->models[i] == NULL) {
      continue;
    }
    cell = &self->cells[i];
    if (cell->type == 0) {
      amount = 0.20000002f;
    } else if (cell->type < 2) {
      amount = 0.09f;
    } else if (cell->type < 3) {
      amount = 0.10000001f;
    }
    cell->moveT = cell->moveT + 1.0f / frames;
    self->models[i]->ambient[3] = cell->alphaFrom - cell->moveT * cell->moveT;
    UiCollectionSphereCellPose(self, i);
    UiCollectionSphereCellScale(self, i, cell->scaleFrom + cell->moveT * cell->moveT * amount);
    if (!(cell->moveT < 1.0f)) {
      self->models[i]->ambient[3] = 0.0f;
      done++;
    }
  }
  return done != 0;
}
