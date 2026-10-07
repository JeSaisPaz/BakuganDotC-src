// bdc 0x0899bc14 UiWorldMapGlobeTweenDone
#include "bdc.h"

/* Advances the globe slide of `UiWorldMap` by 1/16 (`globeTweenTime`) and applies
   it to `mapModel`. Coming in (`out` = 0) with ease e = 1 - (t - 1)^2: ambient alpha
   `globeAlphaBase + e`, root matrix scale `globeScaleBase - e/2`, X `globeTweenStart + 64e`; once
   t reaches 1 (or is NaN) it snaps alpha and scale to 1 and X to `globeTweenEnd`. Going out with
   t^2: alpha `globeAlphaBase - t^2`, scale `globeScaleBase + t^2/2`, X `globeTweenStart - 64t^2`;
   at the end alpha is set to 0. Either way the position is copied into the root matrix
   translation. Returns true when finished. */

bool UiWorldMapGlobeTweenDone(UiScreen *screen, u8 out)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxModel *model;
  bool done = false;
  float e;
  float scale;

  if (out == 0) {
    map->globeTweenTime = map->globeTweenTime + 0.0625f;
    e = map->globeTweenTime - 1.0f;
    map->mapModel->ambient[3] = map->globeAlphaBase + (1.0f - e * e);
    e = map->globeTweenTime - 1.0f;
    scale = map->globeScaleBase - (1.0f - e * e) * 0.5f;
    map->mapModel->data->rootMatrix[10] = scale;
    map->mapModel->data->rootMatrix[5] = scale;
    map->mapModel->data->rootMatrix[0] = scale;
    e = map->globeTweenTime - 1.0f;
    map->mapModel->pos[0] = map->globeTweenStart + (1.0f - e * e) * 64.0f;
    if (!(map->globeTweenTime < 1.0f)) {
      map->mapModel->ambient[3] = 1.0f;
      map->mapModel->data->rootMatrix[10] = 1.0f;
      map->mapModel->data->rootMatrix[5] = 1.0f;
      map->mapModel->data->rootMatrix[0] = 1.0f;
      map->mapModel->pos[0] = map->globeTweenEnd;
      done = true;
    }
  } else {
    map->globeTweenTime = map->globeTweenTime + 0.0625f;
    map->mapModel->ambient[3] = map->globeAlphaBase - map->globeTweenTime * map->globeTweenTime;
    scale = map->globeScaleBase + map->globeTweenTime * map->globeTweenTime * 0.5f;
    map->mapModel->data->rootMatrix[10] = scale;
    map->mapModel->data->rootMatrix[5] = scale;
    map->mapModel->data->rootMatrix[0] = scale;
    map->mapModel->pos[0] =
        map->globeTweenStart - map->globeTweenTime * map->globeTweenTime * 64.0f;
    if (!(map->globeTweenTime < 1.0f)) {
      map->mapModel->ambient[3] = 0.0f;
      done = true;
    }
  }
  model = map->mapModel;
  /* root matrix translation row = pos (lv.q/sv.q copy) */
  model->data->rootMatrix[12] = model->pos[0];
  model->data->rootMatrix[13] = model->pos[1];
  model->data->rootMatrix[14] = model->pos[2];
  model->data->rootMatrix[15] = model->pos[3];
  return done;
}
