// bdc 0x089907d8 UiCollectionFigureMoveModelToDetail
#include "bdc.h"

/* Moves the selected figure model of the figure collection screen (task 314,
   `maybe_UiScreen314Ctor`; 3x2 grid pages of collected metal figures shown as 3D models
   (`"00_P_Dragonoid_N_U_figure.gmo"`…, environment map `"figure_refmap"`), names
   `"cha_spherename_colle_%02d"`, help `"DWCollectionHelp"`; cursor `+0xe78`, page `+0xe79`,
   category `+0xe7d`, entry lists `+0x11c0`) between its cell and the detail position (ease over 16
   frames) while turning it, updating the camera; returns true when done.
   Per frame the cell tween's `t` (`cellTweens[detailCell].t`) advances by 1/16 and is eased
   (`1-(t-1)^2` forward, `t^2` when `back`): the cell camera's screen offset becomes
   (`slideStart`,`slideEnd`) + e*(`moveDx`,`moveDy`), the model's root matrix is reset to a
   half-turn about Y and its rows 0-2 scaled by the eased `moveScaleFrom`→`moveScaleTo` (kept in
   `cellScale[detailCell]`). Once `!(t < 1)` the end state is applied: offset
   (`slideDelta`,`moveToY`) and scale `moveScaleTo`, and the result is true. Every call then runs
   `GfxCameraUpdate``(cam, -1)` and the camera's vtable update hook (slot 2).
   The turn is `vrot` of 3.14f times the bank constant S703 (2/pi), i.e. cos/sin of 3.14f rad. */

bool UiCollectionFigureMoveModelToDetail(UiCollectionFigure *self, bool back)

{
  bool done = false;
  UiTween *tw;
  GfxCamera *cam;
  float *mtx;
  float c;
  float sn;
  int row;
  int col;
  float t;
  float x;
  float y;
  float scale;
  float scl[4];
  const VtblEntry *update;

  tw = &self->cellTweens[self->detailCell];
  t = tw->t + 0.0625f;
  tw->t = t;
  cam = (GfxCamera *)self->cameras[self->detailCell];
  if (back) {
    x = (float)tw->slideStart + t * t * self->moveDx;
    y = (float)tw->slideEnd + t * t * self->moveDy;
  } else {
    x = (float)tw->slideStart + (1.0f - (t - 1.0f) * (t - 1.0f)) * self->moveDx;
    y = (float)tw->slideEnd + (1.0f - (t - 1.0f) * (t - 1.0f)) * self->moveDy;
  }
  GfxCameraSetScreenOffset(x, y, cam);

  /* root matrix = yaw turn of 3.14f rad (vrot of 3.14f * 2/pi quarter turns); rows 1 and 3
     identity */
  c = __builtin_cosf(3.14f);
  sn = __builtin_sinf(3.14f);
  mtx = self->models[self->detailCell]->data->rootMatrix;
  mtx[0] = c;  mtx[1] = 0.0f;  mtx[2] = -sn;  mtx[3] = 0.0f;
  mtx[4] = 0.0f;  mtx[5] = 1.0f;  mtx[6] = 0.0f;  mtx[7] = 0.0f;
  mtx[8] = sn;  mtx[9] = 0.0f;  mtx[10] = c;  mtx[11] = 0.0f;
  mtx[12] = 0.0f;  mtx[13] = 0.0f;  mtx[14] = 0.0f;  mtx[15] = 1.0f;

  t = self->cellTweens[self->detailCell].t;
  if (back) {
    scale = self->moveScaleFrom - t * t * (self->moveScaleFrom - self->moveScaleTo);
  } else {
    scale = self->moveScaleFrom +
            (1.0f - (t - 1.0f) * (t - 1.0f)) * (self->moveScaleTo - self->moveScaleFrom);
  }
  self->cellScale[self->detailCell] = scale;
  mtx = self->models[self->detailCell]->data->rootMatrix;
  scl[0] = scale;
  scl[1] = scale;
  scl[2] = scale;
  scl[3] = 0.0f;
  for (row = 0; row < 3; row++) {
    for (col = 0; col < 4; col++) {
      mtx[row * 4 + col] = mtx[row * 4 + col] * scl[row];
    }
  }

  if (!(self->cellTweens[self->detailCell].t < 1.0f)) {
    tw = &self->cellTweens[self->detailCell];
    GfxCameraSetScreenOffset((float)tw->slideDelta, (float)tw->moveToY,
                             (GfxCamera *)self->cameras[self->detailCell]);

    mtx = self->models[self->detailCell]->data->rootMatrix;
    mtx[0] = c;  mtx[1] = 0.0f;  mtx[2] = -sn;  mtx[3] = 0.0f;
    mtx[4] = 0.0f;  mtx[5] = 1.0f;  mtx[6] = 0.0f;  mtx[7] = 0.0f;
    mtx[8] = sn;  mtx[9] = 0.0f;  mtx[10] = c;  mtx[11] = 0.0f;
    mtx[12] = 0.0f;  mtx[13] = 0.0f;  mtx[14] = 0.0f;  mtx[15] = 1.0f;

    scale = self->moveScaleTo;
    self->cellScale[self->detailCell] = scale;
    mtx = self->models[self->detailCell]->data->rootMatrix;
    scl[0] = scale;
    scl[1] = scale;
    scl[2] = scale;
    scl[3] = 0.0f;
    for (row = 0; row < 3; row++) {
      for (col = 0; col < 4; col++) {
        mtx[row * 4 + col] = mtx[row * 4 + col] * scl[row];
      }
    }
    done = true;
  }

  GfxCameraUpdate((GfxCamera *)self->cameras[self->detailCell], 0xffffffff);
  cam = (GfxCamera *)self->cameras[self->detailCell];
  update = &((const VtblEntry *)cam->base.vtable)[2];
  ((void (*)(void *))update->fn)((u8 *)cam + update->delta);
  return done;
}
