// bdc 0x088ef4f4 GameEventRequestRemove
#include "bdc.h"

/* Sets the event state `+0x264` to 6 (remove the task on the next update). Called by
   `GameFieldDtor`. */

void GameEventRequestRemove(GameEvent *self)

{
  self->state = '\x06';
  return;
}

