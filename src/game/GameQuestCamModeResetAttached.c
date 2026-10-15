// bdc 0x088fd4ac GameQuestCamModeResetAttached
#include "bdc.h"

/* Runs the update (vtable slot 4, `+0x24`) of the quest camera mode's current sub-state (`state`,
   `+0x90`, a `GameQuestCamSubState`) with the caller's `move` vector and `dt`, passed on
   untouched (`a1`/`f12`). */

void GameQuestCamModeResetAttached(GameQuestCamModeBase *self, const float *move, float dt)

{
  GameQuestCamSubState *obj = (GameQuestCamSubState *)self->state;
  const VtblEntry *e = &obj->vtbl[4];

  ((void (*)(void *, const float *, float))e->fn)((char *)obj + e->delta, move, dt);
}
