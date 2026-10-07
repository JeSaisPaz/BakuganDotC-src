// bdc 0x088f0050 GameEvent470Slot7
#include "bdc.h"

/* Vtable slot 7 override of the task-470 event: clears the face expressions of all placed
   characters (`GameFieldCharSetClearFaces`), clears bit 0 of `+0x2d6` and runs the base `GameEventSlot7`. */

void GameEvent470Slot7(GameEvent470 *self)

{
  GameFieldCharSetClearFaces(g_gameFieldCharSet);
  self->flags470 = self->flags470 & 0xfe;
  GameEventSlot7(&self->base);
  return;
}

