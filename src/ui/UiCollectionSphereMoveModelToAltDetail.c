// bdc 0x08981178 UiCollectionSphereMoveModelToAltDetail
#include "bdc.h"

/* Byte-identical copy of `UiCollectionSphereMoveModelToDetail`, used by sub-states 0x10/0x13
   of `UiCollectionSphereMainPhase`. Moves the selected model (`detailCell`) of the sphere (Bakugan figure) collection screen
   (task 312, `maybe_UiScreen312Ctor`) one step (1/16) between its cell and the detail
   position: advances the cell's `moveT`, places the camera's screen offset
   (`GfxCameraSetScreenOffset`) at `from + ease(t) * detailMoveDelta` (ease-out
   `1 - (t-1)^2` going in, `t^2` when `back`), rebuilds the model's root matrix as a yaw turn
   of 3.14f rad and scales its three rows by `detailScaleFrom` eased towards
   `detailScaleTo` (back: `from - t^2 * (from - to)`), also kept in `modelScale`. Once
   `moveT` is no longer below 1 it snaps the offset to the cell's `toX`/`toY` and the scale to
   `detailScaleTo`. Then updates the camera (`GfxCameraUpdate`, all flags) and calls its
   update hook (vtable `+0x10`, `GfxCameraOnUpdate`). Returns true once the move is done. */

typedef struct SphereCamSlot {
  s16 adjust;
  s16 pad;
  void (*fn)(void *);
} SphereCamSlot;

typedef struct SphereCamVTable {
  u8 unk00[0x10];
  SphereCamSlot update;
} SphereCamVTable;

bool UiCollectionSphereMoveModelToAltDetail(UiCollectionSphere *self, bool back)
{
  float vec[4];
  float c;
  float sn;
  int row;
  int col;
  UiCollectionSphereCell *cell;
  const SphereCamSlot *slot;
  GfxCamera *cam;
  float *mtx;
  float t;
  float e;
  float scale;
  s8 idx;
  bool done;

  done = false;
  idx = self->detailCell;
  cell = &self->cells[idx];
  t = cell->moveT + 0.0625f;
  cell->moveT = t;
  if (back) {
    GfxCameraSetScreenOffset((float)cell->fromX + t * t * self->detailMoveDelta[0],
                             (float)cell->fromY + t * t * self->detailMoveDelta[1],
                             (GfxCamera *)self->cameras[idx]);
  } else {
    GfxCameraSetScreenOffset(
        (float)cell->fromX + (1.0f - (t - 1.0f) * (t - 1.0f)) * self->detailMoveDelta[0],
        (float)cell->fromY + (1.0f - (t - 1.0f) * (t - 1.0f)) * self->detailMoveDelta[1],
        (GfxCamera *)self->cameras[idx]);
  }
  mtx = self->models[self->detailCell]->data->rootMatrix;
  /* yaw turn of 3.14f rad (vrot of 3.14f * 2/pi quarter turns); rows 1 and 3 identity */
  c = __builtin_cosf(3.14f);
  sn = __builtin_sinf(3.14f);
  mtx[0] = c;  mtx[1] = 0.0f;  mtx[2] = -sn;  mtx[3] = 0.0f;
  mtx[4] = 0.0f;  mtx[5] = 1.0f;  mtx[6] = 0.0f;  mtx[7] = 0.0f;
  mtx[8] = sn;  mtx[9] = 0.0f;  mtx[10] = c;  mtx[11] = 0.0f;
  mtx[12] = 0.0f;  mtx[13] = 0.0f;  mtx[14] = 0.0f;  mtx[15] = 1.0f;

  scale = self->detailScaleFrom;
  idx = self->detailCell;
  t = self->cells[idx].moveT;
  if (back) {
    scale = scale - t * t * (scale - self->detailScaleTo);
  } else {
    e = t - 1.0f;
    scale = scale + (1.0f - e * e) * (self->detailScaleTo - scale);
  }
  self->modelScale[idx] = scale;
  vec[0] = scale;
  vec[1] = scale;
  vec[2] = scale;
  vec[3] = 0.0f;
  mtx = self->models[idx]->data->rootMatrix;
  /* scale the first three rows by vec[0..2] */
  for (row = 0; row < 3; row++) {
    for (col = 0; col < 4; col++) {
      mtx[row * 4 + col] = mtx[row * 4 + col] * vec[row];
    }
  }

  if (!(self->cells[self->detailCell].moveT < 1.0f)) {
    idx = self->detailCell;
    GfxCameraSetScreenOffset((float)self->cells[idx].toX, (float)self->cells[idx].toY,
                             (GfxCamera *)self->cameras[idx]);
    mtx = self->models[self->detailCell]->data->rootMatrix;
    /* same yaw turn as above */
    mtx[0] = c;  mtx[1] = 0.0f;  mtx[2] = -sn;  mtx[3] = 0.0f;
    mtx[4] = 0.0f;  mtx[5] = 1.0f;  mtx[6] = 0.0f;  mtx[7] = 0.0f;
    mtx[8] = sn;  mtx[9] = 0.0f;  mtx[10] = c;  mtx[11] = 0.0f;
    mtx[12] = 0.0f;  mtx[13] = 0.0f;  mtx[14] = 0.0f;  mtx[15] = 1.0f;
    scale = self->detailScaleTo;
    idx = self->detailCell;
    self->modelScale[idx] = scale;
    vec[0] = scale;
    vec[1] = scale;
    vec[2] = scale;
    vec[3] = 0.0f;
    mtx = self->models[idx]->data->rootMatrix;
    for (row = 0; row < 3; row++) {
      for (col = 0; col < 4; col++) {
        mtx[row * 4 + col] = mtx[row * 4 + col] * vec[row];
      }
    }
    done = true;
  }

  GfxCameraUpdate((GfxCamera *)self->cameras[self->detailCell], 0xffffffffu);
  cam = (GfxCamera *)self->cameras[self->detailCell];
  slot = &((const SphereCamVTable *)cam->base.vtable)->update;
  slot->fn((u8 *)cam + slot->adjust);
  return done;
}
