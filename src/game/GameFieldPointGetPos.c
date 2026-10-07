// bdc 0x088d8f20 GameFieldPointGetPos
#include "bdc.h"

/* Looks up the field point whose kind/id `+0x34` (the field `GameFieldPointSet` fills from its
   `kind` argument) equals `id` in the list `0x08abf0a8` and copies its vec4 position (`+0x20`) to
   `out`; writes a zero vector when there is none. */

void GameFieldPointGetPos(float *out, s32 id)
{
  GameFieldPoint *p;

  if (g_gameFieldPointList != NULL) {
    for (p = *g_gameFieldPointList; p != NULL; p = p->next) {
      if (p->kind == id) {
        out[0] = p->pos[0];
        out[1] = p->pos[1];
        out[2] = p->pos[2];
        out[3] = p->pos[3];
        return;
      }
    }
  }
  out[2] = 0.0f;
  out[1] = 0.0f;
  out[0] = 0.0f;
  out[3] = 0.0f;
}
