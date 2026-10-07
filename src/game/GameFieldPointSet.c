// bdc 0x088d8de0 GameFieldPointSet
#include "bdc.h"

/* Initialises a field point: copies the vec4 position `pos` to `+0x20` and stores the kind `+0x34`,
   the id `+0x30` and the heading `+0x3c` (passed in `f12`; `GameFieldPointSpawnEffect` rotates
   kinds 4..7 by it). */

void GameFieldPointSet(void *point, const float *pos, s32 kind, s32 id, float heading)
{
  GameFieldPoint *p = (GameFieldPoint *)point;

  p->pos[0] = pos[0];
  p->pos[1] = pos[1];
  p->pos[2] = pos[2];
  p->pos[3] = pos[3];
  p->kind = kind;
  p->id = id;
  p->heading = heading;
}
