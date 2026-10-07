// bdc 0x088eaf08 GameEventActionListSetDone
#include "bdc.h"

/* Sets the action list state `+0x50` to 2. */

void GameEventActionListSetDone(GameEventActionList *self)

{
  self->state = '\x02';
  return;
}

