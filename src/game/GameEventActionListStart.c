// bdc 0x088eae0c GameEventActionListStart
#include "bdc.h"

/* Resets the action list (count 0, slots cleared), sets its state `+0x50 = 1` and inserts the task
   with priority 100 (`CoreTaskInsert`). */

void GameEventActionListStart(GameEventActionList *self)

{
  self->count = '\0';
  memset(self->actions,0,0x40);
  self->state = '\x01';
  CoreTaskInsert(self,100);
  return;
}

