// bdc 0x08991068 UiCollectionFigureMoveModelToAltDetail
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
   half-turn about Y and its rows 0-2 scaled (VFPU `vscl.q` by {s,s,s,0}) by the eased `moveScaleFrom`→`moveScaleTo` (kept in
   `cellScale[detailCell]`). Once `!(t < 1)` the end state is applied: offset
   (`slideDelta`,`moveToY`) and scale `moveScaleTo`, and the result is true. Every call then runs
   `GfxCameraUpdate``(cam, -1)` and the camera's vtable update hook (slot 2).
   The angle 3.14 is scaled by the VFPU bank constant S703 (2/pi) before `vrot`, so the rotation
   uses cos/sin of 3.14 radians.
   Byte-identical copy of `UiCollectionFigureMoveModelToDetail`, used for the second detail
   view. */

/* Root matrix = rotation about Y by 3.14 rad (VFPU vrot rows {c,0,-s,0}, {0,1,0,0}, {s,0,c,0},
   {0,0,0,1}). */
static void SetHalfTurnY(float *m)
{
  float c = __builtin_cosf(3.14f);
  float s = __builtin_sinf(3.14f);

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
}

/* Rows 0-2 of the matrix scaled by `scale` (VFPU vscl.q by {s,s,s,0}). */
static void ScaleRows3(float *m, float scale)
{
  int i;

  for (i = 0; i < 12; i++) {
    m[i] = m[i] * scale;
  }
}

bool UiCollectionFigureMoveModelToAltDetail(UiCollectionFigure *self, bool back)
{
  bool done = false;
  UiTween *tw;
  GfxCamera *cam;
  float t;
  float x;
  float y;
  float scale;
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

  SetHalfTurnY(self->models[self->detailCell]->data->rootMatrix);

  t = self->cellTweens[self->detailCell].t;
  if (back) {
    scale = self->moveScaleFrom - t * t * (self->moveScaleFrom - self->moveScaleTo);
  } else {
    scale = self->moveScaleFrom +
            (1.0f - (t - 1.0f) * (t - 1.0f)) * (self->moveScaleTo - self->moveScaleFrom);
  }
  self->cellScale[self->detailCell] = scale;
  ScaleRows3(self->models[self->detailCell]->data->rootMatrix, scale);

  if (!(self->cellTweens[self->detailCell].t < 1.0f)) {
    tw = &self->cellTweens[self->detailCell];
    GfxCameraSetScreenOffset((float)tw->slideDelta, (float)tw->moveToY,
                             (GfxCamera *)self->cameras[self->detailCell]);

    SetHalfTurnY(self->models[self->detailCell]->data->rootMatrix);

    scale = self->moveScaleTo;
    self->cellScale[self->detailCell] = scale;
    ScaleRows3(self->models[self->detailCell]->data->rootMatrix, scale);
    done = true;
  }

  GfxCameraUpdate((GfxCamera *)self->cameras[self->detailCell], 0xffffffff);
  cam = (GfxCamera *)self->cameras[self->detailCell];
  update = &((const VtblEntry *)cam->base.vtable)[2];
  ((void (*)(void *))update->fn)((u8 *)cam + update->delta);
  return done;
}
