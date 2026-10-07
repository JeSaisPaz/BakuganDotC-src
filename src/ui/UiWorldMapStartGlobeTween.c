// bdc 0x0899bb48 UiWorldMapStartGlobeTween
#include "bdc.h"

/* Prepares the slide/zoom of the `UiWorldMap` globe model `mapModel`: coming in
   (`out` = 0) moves it 64 px left (start X `globeTweenStart`, final X `globeTweenEnd`) with alpha
   base 0 and scale base 1.5; going out records the slide target 64 px left with alpha/scale base 1.
   Either way it copies the model position quad into the translation row of its GMO root matrix. */

void UiWorldMapStartGlobeTween(UiScreen *screen, u8 out)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxModel *model;
  float *row;

  if (out == 0) {
    map->globeTweenTime = 0.0f;
    map->globeAlphaBase = 0.0f;
    map->globeScaleBase = 1.5f;
    model = map->mapModel;
    map->globeTweenEnd = model->pos[0];
    model->pos[0] = model->pos[0] - 64.0f;
    model = map->mapModel;
    map->globeTweenStart = model->pos[0];
  } else {
    map->globeTweenTime = 0.0f;
    map->globeAlphaBase = 1.0f;
    map->globeScaleBase = 1.0f;
    model = map->mapModel;
    map->globeTweenEnd = model->pos[0] - 64.0f;
    map->globeTweenStart = model->pos[0];
  }
  row = &model->data->rootMatrix[12];
  row[0] = model->pos[0];
  row[1] = model->pos[1];
  row[2] = model->pos[2];
  row[3] = model->pos[3];
}
