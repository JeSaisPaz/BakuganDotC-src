// bdc 0x088ef64c GameEventSlot7
#include "bdc.h"

/* Virtual slot 7 of the field event task base class: calls `GameEventDeleteProps` and, when an event object
   is attached at `+0x3c`, `GameEventActionListStopAll`. Task 470 overrides it with `GameEvent470Slot7`, which runs
   extra cleanup first and then calls this. */

void GameEventSlot7(GameEvent *self)

{
  GameEventDeleteProps(self);
  if (self->actions != (GameEventActionList *)0x0) {
    GameEventActionListStopAll(self->actions);
  }
  return;
}

