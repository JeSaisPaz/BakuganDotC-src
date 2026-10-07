// bdc 0x088eef8c GameEventUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the field event task (shared by task 470): unless suspended
   (bit 1 of `+0x273`), advances the message loader (`GameEventUpdateRequestedMessage` when `+0x269` is 4, else
   `GameEventUpdateMessageSequence`, both using `"mes_f%d_%02d_%s.bin"`), then dispatches on the event state byte
   `+0x264`: 2 → start (`GameEventExecUntilWait`), 3 → step/wait (`GameEventCheckSkip`, `GameEventCheckWaitDone`), 4 →
   run commands (`GameEventRunCommands`, the interpreter), 5 → virtual slot 8 (`GameEventEnd` /
   override), 6 → `GameEventRemoveTask`. */

void GameEventUpdate(GameEvent *self)
{
  if ((self->flags & 2) == 0) {
    if (self->waitType == 4) {
      GameEventUpdateRequestedMessage(self);
    }
    else {
      GameEventUpdateMessageSequence(self);
    }
    switch (self->state) {
    case 0:
    case 1:
      break;
    case 2:
      self->state = (u8)GameEventExecUntilWait(self);
      break;
    case 3:
      GameEventCheckSkip(self);
      GameEventCheckWaitDone(self);
      break;
    case 4:
      GameEventRunCommands(self);
      break;
    case 5: {
      const VtblEntry *end = &((const VtblEntry *)self->base.vtable)[8];
      ((void (*)(void *))end->fn)((u8 *)self + end->delta);
      break;
    }
    case 6:
      GameEventRemoveTask(self);
      break;
    }
  }
}
