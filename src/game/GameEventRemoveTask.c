// bdc 0x088eef70 GameEventRemoveTask
#include "bdc.h"

/* Event state 6 handler: removes the task (`CoreTaskRemove` with delete). */

void GameEventRemoveTask(GameEvent *self)

{
  CoreTaskRemove(&self->base,true);
  return;
}

