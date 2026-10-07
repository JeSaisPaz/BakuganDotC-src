// bdc 0x088c10e0 GameFieldIsPlayerInFront
#include "bdc.h"

/* Returns 1 when the direction from `obj` (a field gimmick, GfxModel-based) to the player
   (`ActorFindPlayer`) is within 60° of `obj`'s facing axis: the model root matrix applied to
   (0, 0, 1), or (0, 0, -1) when `back`; both vectors normalised (1/sqrt of the squared length, a
   zero vector scaled by 0, each lane clamped to [-1, 1]). Returns 0 when dot(axis, dir) < cos(pi/3),
   so NaN also returns 1. */

static void NormaliseClamped(float v[3])
{
  float len2 = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
  float scale = VfRsq(len2);

  if (len2 == 0.0f) {
    scale = 0.0f;
  }
  v[0] = VfSat1(v[0] * scale);
  v[1] = VfSat1(v[1] * scale);
  v[2] = VfSat1(v[2] * scale);
}

s32 GameFieldIsPlayerInFront(CoreTask *task, GameGimmick *obj, bool back)
{
  GfxModel *model = &obj->base;
  GfxModel *player;
  const float *m;
  float local[3];
  float axis[3];
  float dir[3];
  float dot;
  float limit;

  (void)task;
  local[0] = 0.0f;
  local[1] = 0.0f;
  local[2] = 1.0f;
  if (back) {
    local[2] = -1.0f;
  }
  /* Root matrix rows 0..2 times the local axis (vtfm3, E form). */
  m = model->data->rootMatrix;
  axis[0] = m[0] * local[0] + m[4] * local[1] + m[8] * local[2];
  axis[1] = m[1] * local[0] + m[5] * local[1] + m[9] * local[2];
  axis[2] = m[2] * local[0] + m[6] * local[1] + m[10] * local[2];
  NormaliseClamped(axis);

  player = (GfxModel *)ActorFindPlayer();
  dir[0] = player->pos[0] - model->pos[0];
  dir[1] = player->pos[1] - model->pos[1];
  dir[2] = player->pos[2] - model->pos[2];
  NormaliseClamped(dir);

  dot = axis[0] * dir[0] + axis[1] * dir[1] + axis[2] * dir[2];
  limit = __builtin_cosf(1.04719758f); /* pi/3 */
  if (dot < limit) {
    return 0;
  }
  return 1;
}
