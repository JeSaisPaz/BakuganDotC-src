// bdc 0x088ea554 GameEventPropSetPos
#include "bdc.h"

/* Moves the prop's model to `pos` given in 1/4096 units x 20 (field placement convention).
   Lane 3 of the model position is left as is (the asm stores a stale VFPU lane there). */

void GameEventPropSetPos(GameEventProp *prop, const s32 *pos)
{
  GfxModel *model = (GfxModel *)prop->model;
  float x;
  float y;
  float z;

  if (model != NULL) {
    x = (float)pos[0] * 0.00024414062f;
    y = (float)pos[1] * 0.00024414062f;
    z = (float)pos[2] * 0.00024414062f;
    model->pos[0] = x * 20.0f;
    model->pos[1] = y * 20.0f;
    model->pos[2] = z * 20.0f;
  }
}
