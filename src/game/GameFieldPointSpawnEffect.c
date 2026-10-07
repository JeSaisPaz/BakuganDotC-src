// bdc 0x088d8b6c GameFieldPointSpawnEffect
#include "bdc.h"

/* Spawns the marker effect of a field point by its kind on the effect manager `g_worldEffectMgr`:
   kinds 1..3 effect 3, kinds 4..7 effect 6 rotated by the heading; other kinds mark the point
   disabled and return NULL. Counts spawned effects in `g_gameFieldPointCount`. Kinds 4..7 get the
   effect matrix set to a rotation by `heading` (radians) about the vertical axis. */

void *GameFieldPointSpawnEffect(void *point)
{
  GameFieldPoint *p = point;
  void *effect;
  GfxEffect *fx;
  float heading;

  switch (p->kind) {
  case 1:
  case 2:
  case 3:
    effect = GfxEffectSpawn(g_worldEffectMgr, 3, p->pos);
    break;
  case 4:
  case 5:
  case 6:
  case 7:
    fx = GfxEffectSpawn(g_worldEffectMgr, 6, p->pos);
    effect = fx;
    heading = p->heading;
    fx->matrix[0] = __builtin_cosf(heading);
    fx->matrix[1] = __builtin_sinf(heading);
    fx->matrix[2] = 0.0f;
    fx->matrix[3] = 0.0f;
    fx->matrix[4] = -__builtin_sinf(heading);
    fx->matrix[5] = __builtin_cosf(heading);
    fx->matrix[6] = 0.0f;
    fx->matrix[7] = 0.0f;
    fx->matrix[8] = 0.0f;
    fx->matrix[9] = 0.0f;
    fx->matrix[10] = 1.0f;
    fx->matrix[11] = 0.0f;
    fx->matrix[12] = 0.0f;
    fx->matrix[13] = 0.0f;
    fx->matrix[14] = 0.0f;
    fx->matrix[15] = 1.0f;
    break;
  default:
    p->disabled = 1;
    return NULL;
  }
  g_gameFieldPointCount = g_gameFieldPointCount + 1;
  return effect;
}
