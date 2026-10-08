// bdc 0x088ea71c GameEventPropSetScale
#include "bdc.h"

/* Stores the uniform scale `scale` (1/4096 units) in `+4..+0xc` and applies `scale/4096 * 0.1` to
   the model scale `+0x40`. Lane 3 of the model scale is left as is (the asm stores a stale VFPU
   lane there). */

void GameEventPropSetScale(GameEventProp *prop, s32 scale)
{
  GfxModel *model;
  float x;
  float y;
  float z;

  prop->scaleX = scale;
  prop->scaleY = scale;
  prop->scaleZ = scale;
  if (prop->model != NULL) {
    model = (GfxModel *)prop->model;
    x = (float)prop->scaleX * 0.00024414062f;
    y = (float)prop->scaleY * 0.00024414062f;
    z = (float)prop->scaleZ * 0.00024414062f;
    model->scale[0] = x * 0.1f;
    model->scale[1] = y * 0.1f;
    model->scale[2] = z * 0.1f;
  }
}
